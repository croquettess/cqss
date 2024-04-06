#ifndef CQSS_NET_SERVER_ENGINE_H_
#define CQSS_NET_SERVER_ENGINE_H_

#include <memory>

#include "cqss/cmn/design/singleton.hpp"
#include "cqss/cmn/errc/errc.hpp"

#include "server.h"

namespace cqss {
namespace net {

class ServerEngine : public cqss::cmn::design::ISingleton<ServerEngine> {
  friend cqss::cmn::design::ISingleton<ServerEngine>;

 public:
  ServerEngine();
  ~ServerEngine() = default;
  virtual std::error_code Init(const char *proc_name, const char *cfg_path);
  virtual std::error_code UnInit();
  virtual std::error_code StartUp();
  virtual std::error_code Shutdown();

 private:
  std::shared_ptr<Server> server_;
};

}  // namespace net
}  // namespace cqss

#endif  // CQSS_NET_SERVER_ENGINE_H_
