#include "server_log.h"

#include "glog/logging.h"

using namespace std;
using namespace cqss::net;
using namespace cqss::cmn;

error_code Log::Init(const char *proc_name) {
  std::error_code ec;
  google::InitGoogleLogging(proc_name);
  google::SetStderrLogging(google::INFO);
  google::LogToStderr();
  LOG(INFO) << "Log init successful";
  return ec;
}
