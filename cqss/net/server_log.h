#ifndef CQSS_NET_SERVER_LOG_H_
#define CQSS_NET_SERVER_LOG_H_

#include "glog/logging.h"

#include "cqss/cmn/errc/errc.hpp"

namespace cqss {
namespace net {

class Log {
 public:
  static std::error_code Init(const char *proc_name);
  static std::error_code UnInit();
};

}  // namespace net
}  // namespace cqss

#endif  // CQSS_NET_SERVER_LOG_H_
