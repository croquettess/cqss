#include "server.h"

#include <cerrno>
#include <unordered_map>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>

#include "cqss/cmn/errc/errc.hpp"

#include "server_log.h"

using namespace std;
using namespace cqss::net;
using namespace cqss::cmn::err;

ConnInfo::ConnInfo() : fd(-1), is_close(false) {
  addr_len = sizeof(addr);
  memset(&addr, 0, addr_len);
}

Server::Server()
    : socket_(-1), listen_queue_len_(0), rd_buffer_len_(0), wt_buffer_len_(0) {}

error_code Server::Init(ServerConfig &config) {
  error_code ec;
  LOG(INFO) << "Server init";

  if (config.GetProtocol() == ServerProtocol::kTcp) {
    if ((socket_ = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
      LOG(ERROR) << "Create socket failed, msg: " << strerror(errno);
      return error_code(errno, generic_category());
    }
  } else {
    LOG(ERROR) << "Invaild Protocal";
    return make_error_code(Errc::kInvaildProtocol);
  }
  LOG(INFO) << "Create socket successful, socket fd: " << socket_;

  sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  const auto &ip = config.GetIp();
  if (inet_aton(ip.data(), &addr.sin_addr) == 0) {
    LOG(ERROR) << "Invaild address: " << ip;
    return make_error_code(Errc::kInvaildAddress);
  }
  const auto &port = config.GetPort();
  addr.sin_port = htons(port);
  if (bind(socket_, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) == -1) {
    close(socket_);
    LOG(ERROR) << "Bind failed, msg: " << strerror(errno);
    return error_code(errno, generic_category());
  }
  LOG(INFO) << "Bind address successful, domain server: " << ip << ":" << port;

  listen_queue_len_ = config.GetListenQueuelen();
  LOG(INFO) << "Listen queue size: " << listen_queue_len_;

  rd_buffer_len_ = config.GetRdBufferlen();
  LOG(INFO) << "Read buffer size: " << rd_buffer_len_;

  wt_buffer_len_ = config.GetWtBufferLen();
  LOG(INFO) << "Write buffer size: " << wt_buffer_len_;

  return ec;
}

error_code Server::StartUp() {
  error_code ec;
  if (listen(socket_, listen_queue_len_) == -1) {
    LOG(ERROR) << "Listen fail, msg: " << strerror(errno);
    return error_code(errno, generic_category());
  }
  LOG(INFO) << "Listening...";

  ec = Select();
  if (ec) {
    LOG(ERROR) << "Enable select model failed";
    return ec;
  }

  return ec;
}

error_code Server::Shutdown() {
  error_code ec;
  close(socket_);
  LOG(INFO) << "Close server, socket fd: " << socket_;
  return ec;
}

error_code Server::Select() {
  error_code ec;

  fd_set rset, crset;
  FD_ZERO(&rset);
  FD_SET(socket_, &rset);
  int nfds = socket_;
  int nready;

  ConnInfo info;
  unordered_map<int, ConnInfo> infos;
  while (true) {
    crset = rset;
    nready = select(nfds + 1, &crset, nullptr, nullptr, nullptr);
    for (int fd = socket_; nready > 0; ++fd) {
      if (FD_ISSET(fd, &crset)) {
        if (fd == socket_) {
          ec = Accept(info);
          if (ec) {
            return ec;
          }
          auto &fd = info.fd;
          FD_SET(fd, &rset);
          if (fd > nfds) {
            nfds = fd;
          }
          infos.emplace(make_pair(fd, info));
          LOG(INFO) << "Register " << info.GetEndpoints() << " to rest";
        } else {
          if (infos.find(fd) == infos.end()) {
            return make_error_code(Errc::kInvaildValue);
          }
          auto &info = infos[fd];
          ec = ProcIO(info);
          if (ec) {
            close(info.fd);
            info.is_close = true;
            FD_CLR(info.fd, &rset);
            LOG(INFO) << "Deregister " << info.GetEndpoints() << " from rset";
          }
        }
        --nready;
      }
    }
  }
}

error_code Server::Accept(ConnInfo &info) {
  error_code ec;
  auto &fd = info.fd;
  auto &addr = info.addr;
  auto &addr_len = info.addr_len;
  fd = accept(socket_, reinterpret_cast<sockaddr *>(&addr), &addr_len);
  if (fd == -1) {
    LOG(ERROR) << "Accept a connection failed, msg:" << strerror(errno);
    return error_code(errno, generic_category());
  }
  LOG(INFO) << "Accept a connection , endpoints: "
            << info.GetEndpoints() << ", socket fd: " << info.fd;
  return ec;
}

error_code Server::ProcIO(ConnInfo &info) {
  error_code ec;
  const auto &fd = info.fd;

  char rd_buffer[rd_buffer_len_];
  auto recv_len = recv(fd, rd_buffer, rd_buffer_len_ - 1, 0);
  rd_buffer[recv_len] = '\0';
  if (recv_len > 0) {
    LOG(INFO) << "Recv from " << info.GetEndpoints() << ", msg: " << rd_buffer;

    char wt_buffer[wt_buffer_len_];
    strncpy(wt_buffer, rd_buffer, recv_len+1);

    send(fd, wt_buffer, strlen(wt_buffer), 0);
  } else if (recv_len == 0) {
    LOG(INFO) << "Close connection, endpoints: " << info.GetEndpoints();
    return make_error_code(Errc::kCloseConnection);
  } else {
    LOG(ERROR) << "Recv from " << info.GetEndpoints() << " failed, msg" << strerror(errno);
    return error_code(errno, generic_category());
  }

  return ec;
}
