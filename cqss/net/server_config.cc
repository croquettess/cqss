#include "server_config.h"

#include <fstream>

#include "rapidjson/error/en.h"
#include "rapidjson/istreamwrapper.h"

#include "server_log.h"

using namespace std;
using namespace cqss::cmn::err;
using namespace rapidjson;

#define MODEL "model"
#define DOMAIN_SERVER "domain_server"
#define LISTEN_QUEUE_LEN "listen_queue_len"
#define RD_BUFFER_LEN "rd_buffer_len"
#define WT_BUFFER_LEN "wt_buffer_len"

ServerConfig::ServerConfig()
    : model_(""),
      domain_server_(""),
      listen_queue_len_(0),
      rd_buffer_len_(0),
      wt_buffer_len_(0),
      is_load_(false){};

error_code ServerConfig::Init(const char *cfg_path) {
  error_code ec;
  
  LOG(INFO) << "Init config, cfg_path: " << cfg_path;
  Document doc;
  do {
    ec = Read(cfg_path, doc);
    if (ec) { break; }

    ec = Load(doc);
    if (ec) { break; }

    LOG(INFO) << "Config init successful";
    return ec;
  } while(false);

  LOG(ERROR) << "Config init fail";
  return ec;
}

error_code ServerConfig::Read(const char *cfg_path, Document &doc) {
  error_code ec;
  ifstream ifs(cfg_path);
  if (!ifs.is_open()) {
    LOG(ERROR) << "Could not open file for reading";
    return make_error_code(Errc::kVaildCfg);
  }
  IStreamWrapper isw(ifs);
  doc.ParseStream(isw);

  if (doc.HasParseError()) {
    LOG(ERROR) << "Parse cfg error, path: " << cfg_path
               << ", error: " << GetParseError_En(doc.GetParseError())
               << ", offset: " << doc.GetErrorOffset();
    return std::make_error_code(Errc::kVaildCfg);
  }

  if (!doc.IsObject()) {
    LOG(ERROR) << "Is not an json object, error: "
               << GetParseError_En(doc.GetParseError());
    return std::make_error_code(Errc::kVaildCfg);
  }

  LOG(INFO) << "Read cfg successful";
  return ec;
}

error_code ServerConfig::Load(Document &doc) {
  error_code ec;
  if (!doc.HasMember(MODEL) || !doc[MODEL].IsString()) {
    LOG(ERROR) << "Vaild key: " << MODEL;
    return make_error_code(Errc::kVaildKey);
  }
  model_ = doc[MODEL].GetString();
  LOG(INFO) << "Parse object, " << MODEL << ": " << model_;

  if (!doc.HasMember(DOMAIN_SERVER) || !doc[DOMAIN_SERVER].IsString()) {
    LOG(ERROR) << "Vaild key: " << DOMAIN_SERVER;
    return make_error_code(Errc::kVaildKey);
  }
  domain_server_ = doc[DOMAIN_SERVER].GetString();
  LOG(INFO) << "Parse object, " << DOMAIN_SERVER << ": " << domain_server_;

  if (!doc.HasMember(LISTEN_QUEUE_LEN) || !doc[LISTEN_QUEUE_LEN].IsUint()) {
    LOG(ERROR) << "Vaild key: " << LISTEN_QUEUE_LEN;
    return make_error_code(Errc::kVaildKey);
  }
  listen_queue_len_ = doc[LISTEN_QUEUE_LEN].GetUint();
  LOG(INFO) << "Parse object, " << LISTEN_QUEUE_LEN << ": "
            << listen_queue_len_;

  if (!doc.HasMember(RD_BUFFER_LEN) || !doc[RD_BUFFER_LEN].IsUint()) {
    LOG(ERROR) << "Vaild key: " << RD_BUFFER_LEN;
    return make_error_code(Errc::kVaildKey);
  }
  rd_buffer_len_ = doc[RD_BUFFER_LEN].GetUint();
  LOG(INFO) << "Parse object, " << RD_BUFFER_LEN << ": " << rd_buffer_len_;

  if (!doc.HasMember(WT_BUFFER_LEN) || !doc[WT_BUFFER_LEN].IsUint()) {
    LOG(ERROR) << "Vaild key: " << WT_BUFFER_LEN;
    return make_error_code(Errc::kVaildKey);
  }
  wt_buffer_len_ = doc[WT_BUFFER_LEN].GetUint();
  LOG(INFO) << "Parse object, " << WT_BUFFER_LEN << ": " << wt_buffer_len_;

  LOG(INFO) << "Load cfg successful";
  return ec;
}
