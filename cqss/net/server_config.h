#ifndef CQSS_NET_SERVER_CONFIG_H_
#define CQSS_NET_SERVER_CONFIG_H_

#include <cstdint>
#include <string>

#include "rapidjson/document.h"

#include "cqss/cmn/design/singleton.hpp"
#include "cqss/cmn/errc/errc.hpp"

namespace cqss {
namespace net {

enum class ServerProtocol { kUnknown, kTcp, kUdp };

enum class ServerModel { kUnKnown, kSelect, kPoll, kEpoll };

class ServerConfig {
 public:
  ServerConfig();
  ~ServerConfig() = default;
  std::error_code Init(const char *cfg_path);

  inline const ServerProtocol &GetProtocol() { return protocol_; }
  inline const ServerModel &GetModel() { return model_; }
  inline const std::string &GetIp() { return ip_; }
  inline const size_t &GetPort() { return port_; }
  inline const size_t &GetListenQueuelen() { return listen_queue_len_; }
  inline const size_t &GetRdBufferlen() { return rd_buffer_len_; }
  inline const size_t &GetWtBufferLen() { return wt_buffer_len_; }

 protected:
  virtual std::error_code Parse(rapidjson::Document &doc);

 private:
  std::error_code Read(const char *cfg_path, rapidjson::Document &doc);

 protected:
  ServerProtocol protocol_;
  ServerModel model_;
  std::string ip_;
  size_t port_;
  size_t listen_queue_len_;
  size_t rd_buffer_len_;
  size_t wt_buffer_len_;
};

}  // namespace net
}  // namespace cqss

#endif  // CQSS_NET_SERVER_CONFIG_H_
