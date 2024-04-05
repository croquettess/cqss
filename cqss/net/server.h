#ifndef CQSS_NET_SERVER_H_
#define CQSS_NET_SERVER_H_

#include <string>

#include <arpa/inet.h>

#include "cqss/cmn/errc/errc.hpp"

#include "server_config.h"

namespace cqss {
namespace net {

class Server {
public:
  Server();
  ~Server() = default;
  std::error_code Init(ServerConfig &config);
  std::error_code Listen();

 private:
  int socket_;
  uint16_t listen_queue_len_;
  uint16_t rd_buffer_len_;
  uint16_t wt_buffer_len_;
};

}
}  // namespace cqss

#endif  // CQSS_NET_SERVER_H_
