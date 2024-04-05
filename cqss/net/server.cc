#include "server.h"

#include <cerrno>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "cqss/cmn/errc/errc.hpp"

#include "server_log.h"

using namespace std;
using namespace cqss::net;
using namespace cqss::cmn::err;

Server::Server()
    : socket_(-1), listen_queue_len_(0), rd_buffer_len_(0), wt_buffer_len_(0) {}

error_code Server::Init(ServerConfig &config) {
  error_code ec;
  LOG(INFO) << "Server init";

  if (config.GetProtocol() == ServerProtocol::kTcp) {
    if ((socket_ = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
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

error_code Server::Listen() {
  error_code ec;
  if (listen(socket_, listen_queue_len_) == -1) {
    LOG(ERROR) << "Listen fail, msg: " << strerror(errno);
    return error_code(errno, generic_category());
  }
  LOG(INFO) << "Listening...";
  return ec;
}
