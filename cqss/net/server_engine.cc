#include "server_engine.h"

#include "server_config.h"
#include "server_log.h"

using namespace std;
using namespace cqss::net;

ServerEngine::ServerEngine() { server_ = make_shared<Server>(); }

error_code ServerEngine::Init(const char *proc_name, const char *cfg_path) {
  error_code ec;

  ec = Log::Init(proc_name);
  if (ec) {
    LOG(ERROR) << "Log init failed";
    return ec;
  }
  LOG(INFO) << "Log init successful";

  ServerConfig config;
  ec = config.Init(cfg_path);
  if (ec) {
    LOG(ERROR) << "Config init failed";
    return ec;
  }
  LOG(INFO) << "Config init successful";

  ec = server_->Init(config);
  if (ec) {
    LOG(ERROR) << "Server init failed";
    return ec;
  }
  LOG(INFO) << "Server init successful";

  return ec;
}

error_code ServerEngine::UnInit() {
  error_code ec;
  return ec;
}

error_code ServerEngine::StartUp() {
  error_code ec;
  ec = server_->StartUp();
  if (ec) {
    LOG(ERROR) << "Server turns on the listening state failed";
  }
  LOG(INFO) << "Server turns on the listening state successful";
  return ec;
}

error_code ServerEngine::Shutdown() {
  error_code ec;
  ec = server_->Shutdown();
  if (ec) {
    LOG(ERROR) << "Server closes listening state failed";
  }
  LOG(INFO) << "Server closes listening state successful";
  return ec;
}
