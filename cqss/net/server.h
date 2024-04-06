#ifndef CQSS_NET_SERVER_H_
#define CQSS_NET_SERVER_H_

#include <string>

#include <arpa/inet.h>
#include <sys/select.h>

#include "cqss/cmn/errc/errc.hpp"

#include "server_config.h"

namespace cqss {
namespace net {

struct ConnInfo {
  ConnInfo();
  ~ConnInfo() = default;
  inline std::string GetEndpoints() { return std::string(inet_ntoa(addr.sin_addr)) + ":" + std::to_string(ntohs(addr.sin_port)); }
  int fd;
  sockaddr_in addr;
  socklen_t addr_len;
  bool is_close;
};

class Server {
 public:
  Server();
  ~Server() = default;
  std::error_code Init(ServerConfig &config);
  std::error_code StartUp();
  std::error_code Shutdown();

 private:
  std::error_code Select();
  std::error_code SelectAccept(ConnInfo &info, fd_set &rset, int &nfds);
  std::error_code Accept(ConnInfo &info);
  std::error_code ProcIO(ConnInfo &info);

 private:
  int socket_;
  uint16_t listen_queue_len_;
  uint16_t rd_buffer_len_;
  uint16_t wt_buffer_len_;
};

}  // namespace net
}  // namespace cqss

#endif  // CQSS_NET_SERVER_H_
