#include <iostream>

#include "server_engine.h"

using namespace cqss::net;

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage : " << argv[0] << " $cfg_path" << std::endl;
    return 1;
  }

  const char *proc_name = argv[0];
  const char *cfg_path = argv[1];

  auto &engine = ServerEngine::Instance();
  engine.Init(proc_name, cfg_path);
  engine.UnInit();

  return 0;
}
