#ifndef CQSS_NET_SERVER_ENGINE_H_
#define CQSS_NET_SERVER_ENGINE_H_

#include "cqss/cmn/design/singleton.hpp"
#include "cqss/cmn/errc/errc.hpp"

namespace cqss {
namespace net {

class ServerEngine : public cqss::cmn::design::ISingleton<ServerEngine> {
  friend cqss::cmn::design::ISingleton<ServerEngine>;

 public:
  virtual std::error_code Init(const char *proc_name, const char *cfg_path);
  virtual std::error_code UnInit();
};

}  // namespace net
}  // namespace cqss

#endif  // CQSS_NET_SERVER_ENGINE_H_
