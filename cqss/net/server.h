#ifndef CQSS_NET_SERVER_H_
#define CQSS_NET_SERVER_H_

#include <string>

#include <arpa/inet.h>
#include <sys/select.h>
#include <functional>

#include "cqss/cmn/errc/errc.hpp"

#include "server_config.h"

namespace cqss {
namespace net {

struct SocketInfo {
  SocketInfo();
  ~SocketInfo() = default;
  inline std::string GetEndpoints() {
    return std::string(inet_ntoa(addr.sin_addr)) + ":" +
           std::to_string(ntohs(addr.sin_port));
  }
  int fd;
  sockaddr_in addr;
  socklen_t addr_len;
  bool is_close;
};

class Server {
  using proc_io_cb_t = std::function<std::error_code(
      const char *rd_buffer, const size_t &rd_buffer_len, char *wt_buffer,
      size_t &wt_buffer_len)>;

 public:
  Server(const proc_io_cb_t &cb =
             [](const char *rd_buffer, const size_t &rd_buffer_len,
                char *wt_buffer, size_t &wt_buffer_len) -> std::error_code {
    std::error_code ec;
    strncpy(wt_buffer, rd_buffer, rd_buffer_len + 1);
    wt_buffer_len = strlen(wt_buffer);
    return ec;
  });
  ~Server() = default;

  std::error_code Init(ServerConfig &config);
  std::error_code StartUp();
  std::error_code Shutdown();

  inline void SetProcIOCb(const proc_io_cb_t &cb) { proc_io_cb_ = cb; }

 private:
  std::error_code Select();
  std::error_code Accept(SocketInfo &info);
  std::error_code ProcIO(SocketInfo &info);

 private:
  int socket_;
  uint16_t listen_queue_len_;
  uint16_t rd_buffer_len_;
  uint16_t wt_buffer_len_;
  proc_io_cb_t proc_io_cb_;
};

}  // namespace net
}  // namespace cqss

#endif  // CQSS_NET_SERVER_H_
