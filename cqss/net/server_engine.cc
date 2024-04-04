#include "server_engine.h"

#include "server_config.h"
#include "server_log.h"

using namespace std;
using namespace cqss::net;

std::error_code ServerEngine::Init(const char *proc_name, const char *cfg_path) {
  std::error_code ec;
  do {
    ec = Log::Init(proc_name);
    if (ec) { break; }

    ec = ServerConfig::Instance().Init(cfg_path);
    if (ec) { break; }

    LOG(INFO) << "Engine init successful";
    return ec;
  } while(false);

  LOG(ERROR) << "Engine Init fail";
  return ec;
}

std::error_code ServerEngine::UnInit() {
  std::error_code ec;

  do {
    LOG(INFO) << "Engine uninit successful.";
    return ec;
  } while(false);

  LOG(ERROR) << "Engine uninit fail.";
  return ec;
}
