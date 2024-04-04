#ifndef CQSS_NET_SERVER_CONFIG_H_
#define CQSS_NET_SERVER_CONFIG_H_

#include <cstdint>
#include <string>

#include "rapidjson/document.h"

#include "cqss/cmn/design/singleton.hpp"
#include "cqss/cmn/errc/errc.hpp"

class ServerConfig : public cqss::cmn::design::ISingleton<ServerConfig> {
  friend cqss::cmn::design::ISingleton<ServerConfig>;

 public:
  ServerConfig();
  std::error_code Init(const char *cfg_path);

  inline const std::string &GetDomainServer() { return domain_server_; }
  inline const uint16_t &GetListenQueuelen() { return listen_queue_len_; }
  inline const uint16_t &GetRdBufferlen() { return rd_buffer_len_; }
  inline const uint16_t &GetWtBufferLen() { return wt_buffer_len_; }
  inline const bool &GetIsLoad() { return is_load_; }

 protected:
  virtual std::error_code Load(rapidjson::Document &doc);

 private:
  std::error_code Read(const char *cfg_path, rapidjson::Document &doc);

 protected:
  std::string model_;
  std::string domain_server_;
  uint16_t listen_queue_len_;
  uint16_t rd_buffer_len_;
  uint16_t wt_buffer_len_;
  bool is_load_;
};

#endif  // CQSS_NET_SERVER_CONFIG_H_
