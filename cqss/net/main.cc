#include <iostream>

#include "server_engine.h"
#include "server_log.h"

using namespace cqss::net;

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage : " << argv[0] << " $cfg_path" << std::endl;
    return 1;
  }
  const char *proc_name = argv[0];
  const char *cfg_path = argv[1];

  auto &engine = ServerEngine::Instance();
  if (engine.Init(proc_name, cfg_path)) {
    LOG(ERROR) << "Engine init failed";
    return 1;
  }
  LOG(INFO) << "Engine init successful";

  if (engine.StartUp()) {
    LOG(ERROR) << "Engine startup failed";
    return 1;
  }

  if (engine.Shutdown()) {
    LOG(ERROR) << "Engine shutdown failed";
    return 1;
  }

  if (engine.UnInit()) {
    LOG(ERROR) << "Engine uninit failed";
    return 1;
  }
  LOG(INFO) << "Engine uninit successful";

  return 0;
}
