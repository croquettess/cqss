#include "server.h"

#include <cerrno>
#include <unordered_map>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/epoll.h>

#include "cqss/cmn/errc/errc.hpp"

#include "server_log.h"

using namespace std;
using namespace cqss::net;
using namespace cqss::cmn::err;

SocketInfo::SocketInfo() : fd(-1), is_close(false) {
  addr_len = sizeof(addr);
  memset(&addr, 0, addr_len);
}

Server::Server(const proc_io_cb_t &cb)
    : socket_(-1),
      model_(ServerModel::kUnKnown),
      listen_queue_len_(0),
      rd_buffer_len_(0),
      wt_buffer_len_(0),
      proc_io_cb_(cb) {}

error_code Server::Init(ServerConfig &config) {
  error_code ec;
  LOG(INFO) << "Server init";

  if (config.GetProtocol() == ServerProtocol::kTcp) {
    if ((socket_ = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
      LOG(ERROR) << "Create socket failed, errmsg: " << strerror(errno);
      return make_errno_error_code();
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
    LOG(ERROR) << "Bind failed, errmsg: " << strerror(errno);
    return make_errno_error_code();
  }
  LOG(INFO) << "Bind address successful, domain server: " << ip << ":" << port;

  model_ = config.GetModel();

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
    LOG(ERROR) << "Listen fail, errmsg: " << strerror(errno);
    return make_errno_error_code();
  }
  LOG(INFO) << "Listening...";

  switch (model_) {
    case ServerModel::kUnKnown:
      break;
    case ServerModel::kSelect:
      ec = Select();
      break;
    case ServerModel::kPoll:
      break;
    case ServerModel::kEpoll:
      ec = Epoll();
      break;
    default:
      break;
  }
  if (ec) {
    LOG(ERROR) << "Enable io multiplex failed";
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

  SocketInfo info;
  unordered_map<int, SocketInfo> infos;
  while (true) {
    crset = rset;
    nready = select(nfds + 1, &crset, nullptr, nullptr, nullptr);
    for (int fd = socket_; nready > 0; ++fd) {
      if (FD_ISSET(fd, &crset)) {
        if (fd == socket_) {
          ec = Accept(info);
          if (ec) {
            continue;
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
            FD_CLR(info.fd, &rset);
            close(info.fd);
            info.is_close = true;
            LOG(INFO) << "Deregister " << info.GetEndpoints() << " from rset";
          }
        }
        --nready;
      }
    }
  }
}

error_code Server::Epoll() {
  error_code ec;
  int epfd = epoll_create(1);
  epoll_event epev;
  epev.events = EPOLLIN;
  epev.data.fd = socket_;
  epoll_ctl(epfd, EPOLL_CTL_ADD, socket_, &epev);

  epoll_event epevs[1024];
  memset(&epevs, 0, sizeof(epevs));
  SocketInfo info;
  unordered_map<int, SocketInfo> infos;
  while (true) {
    int nready = epoll_wait(epfd, epevs, 1024, -1);
    for (int i = 0; i < nready; ++i) {
      const auto &fd = epevs[i].data.fd;
      auto &ev = epevs[i].events;
      if (socket_ == fd) {
          ec = Accept(info);
          if (ec) {
            continue;
          }
          epev.events = EPOLLIN;
          epev.data.fd = info.fd;
          epoll_ctl(epfd, EPOLL_CTL_ADD, info.fd, &epev);
          infos.emplace(make_pair(info.fd, info));
          LOG(INFO) << "Register " << info.GetEndpoints() << " to rest";
      } else if (ev & EPOLLIN) {
          if (infos.find(fd) == infos.end()) {
            return make_error_code(Errc::kInvaildValue);
          }
          auto &info = infos[fd];
          ec = ProcIO(info);
          if (ec) {
            epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
            close(fd);
            info.is_close = true;
            LOG(INFO) << "Deregister " << info.GetEndpoints() << " from rset";
          }
      }
    }
  }

  return ec;
}

error_code Server::Accept(SocketInfo &info) {
  error_code ec;
  auto &fd = info.fd;
  auto &addr = info.addr;
  auto &addr_len = info.addr_len;
  fd = accept(socket_, reinterpret_cast<sockaddr *>(&addr), &addr_len);
  if (fd == -1) {
    LOG(ERROR) << "Accept a connection failed, errmsg:" << strerror(errno);
    return make_errno_error_code();
  }
  LOG(INFO) << "Accept a connection , endpoints: " << info.GetEndpoints()
            << ", socket fd: " << info.fd;
  return ec;
}

error_code Server::ProcIO(SocketInfo &info) {
  error_code ec;
  const auto &fd = info.fd;
  const auto &endpoints = info.GetEndpoints();

  char rd_buffer[rd_buffer_len_];
  char wt_buffer[wt_buffer_len_];
  size_t wt_buffer_len;
  auto recv_len = recv(fd, rd_buffer, rd_buffer_len_ - 1, 0);
  rd_buffer[recv_len] = '\0';
  if (recv_len > 0) {
    LOG(INFO) << "Recv from " << endpoints << ", msg: " << rd_buffer;
    ec = proc_io_cb_(rd_buffer, recv_len, wt_buffer, wt_buffer_len);
    if (ec) {
      LOG(ERROR) << "Process msg from " << endpoints
                 << " failed, msg: " << rd_buffer;
      return make_error_code(Errc::kProcessError);
    }

    if (-1 == send(fd, wt_buffer, wt_buffer_len, 0)) {
      LOG(ERROR) << "Send msg to " << endpoints
                 << " failed, errmsg: " << strerror(errno);
      return make_errno_error_code();
    }
  } else if (recv_len == 0) {
    LOG(INFO) << "Close connection, endpoints: " << endpoints;
    return make_error_code(Errc::kCloseConnection);
  } else {
    LOG(ERROR) << "Recv from " << endpoints
               << " failed, errmsg: " << strerror(errno);
    return make_errno_error_code();
  }

  return ec;
}
