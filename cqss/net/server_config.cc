#include "server_config.h"

#include <fstream>
#include <vector>

#include "rapidjson/error/en.h"
#include "rapidjson/istreamwrapper.h"
#include "absl/strings/str_split.h"

#include "server_log.h"

#define PROTOCOL "protocol"
#define TCP "tcp"
#define UDP "udp"
#define MODEL "model"
#define SELECT "select"
#define POLL "poll"
#define EPOLL "epoll"
#define DOMAIN_SERVER "domain_server"
#define IP "ip"
#define PORT "port"
#define LISTEN_QUEUE_LEN "listen_queue_len"
#define RD_BUFFER_LEN "rd_buffer_len"
#define WT_BUFFER_LEN "wt_buffer_len"

using namespace std;
using namespace cqss::cmn::err;
using namespace cqss::net;
using namespace rapidjson;
using namespace absl;

ServerConfig::ServerConfig()
    : protocol_(ServerProtocol::kUnknown),
      model_(ServerModel::kUnKnown),
      ip_(""),
      port_(0),
      listen_queue_len_(0),
      rd_buffer_len_(0),
      wt_buffer_len_(0) {}

error_code ServerConfig::Init(const char *cfg_path) {
  error_code ec;
  LOG(INFO) << "Config Init, cfg_path: " << cfg_path;

  Document doc;
  ec = Read(cfg_path, doc);
  if (ec) {
    LOG(ERROR) << "Read cfg failed";
    return ec;
  }
  LOG(INFO) << "Read cfg successful";

  ec = Load(doc);
  if (ec) {
    LOG(ERROR) << "Load cfg failed";
    return ec;
  }
  LOG(INFO) << "Load cfg successful";

  return ec;
}

error_code ServerConfig::Read(const char *cfg_path, Document &doc) {
  error_code ec;
  LOG(INFO) << "Read cfg";

  ifstream ifs(cfg_path);
  if (!ifs.is_open()) {
    LOG(ERROR) << "Cannot open file for reading";
    return make_error_code(Errc::kInvaildCfg);
  }

  IStreamWrapper isw(ifs);
  doc.ParseStream(isw);
  if (doc.HasParseError()) {
    LOG(ERROR) << "Parse cfg error, path: " << cfg_path
               << ", error: " << GetParseError_En(doc.GetParseError())
               << ", offset: " << doc.GetErrorOffset();
    return std::make_error_code(Errc::kInvaildCfg);
  }
  if (!doc.IsObject()) {
    LOG(ERROR) << "Is not an json object, error: "
               << GetParseError_En(doc.GetParseError());
    return std::make_error_code(Errc::kInvaildCfg);
  }

  return ec;
}

error_code ServerConfig::Load(Document &doc) {
  error_code ec;
  LOG(INFO) << "Load cfg";

  if (!doc.HasMember(PROTOCOL) || !doc[PROTOCOL].IsString()) {
    LOG(ERROR) << "Invaild key: " << PROTOCOL;
    return make_error_code(Errc::kInvaildKey);
  }
  const string protocol = doc[PROTOCOL].GetString();
  if (protocol == TCP) {
    protocol_ = ServerProtocol::kTcp;
  } else if (protocol == UDP) {
    protocol_ = ServerProtocol::kUdp;
  } else {
    LOG(ERROR) << "Invaild protocol: " << protocol;
    return make_error_code(Errc::kInvaildProtocol);
  }
  LOG(INFO) << "Parse object, " << PROTOCOL << ": " << protocol;

  if (protocol_ == ServerProtocol::kTcp) {
    if (!doc.HasMember(MODEL) || !doc[MODEL].IsString()) {
      LOG(ERROR) << "Invaild key: " << MODEL;
      return make_error_code(Errc::kInvaildKey);
    }
    const string model = doc[MODEL].GetString();
    if (model == SELECT) {
      model_ = ServerModel::kSelect;
    } else if (model == POLL) {
      model_ = ServerModel::kPoll;
    } else if (model == EPOLL) {
      model_ = ServerModel::kEpoll;
    } else {
      LOG(ERROR) << "Invaild model: " << model;
      return make_error_code(Errc::kInvaildModel);
    }
    LOG(INFO) << "Parse object, " << MODEL << ": " << model;
  }

  if (!doc.HasMember(DOMAIN_SERVER) || !doc[DOMAIN_SERVER].IsString()) {
    LOG(ERROR) << "Invaild key: " << DOMAIN_SERVER;
    return make_error_code(Errc::kInvaildKey);
  }
  const auto &domain_server = doc[DOMAIN_SERVER].GetString();
  const auto &tmp = static_cast<vector<string>>(StrSplit(domain_server, ':'));
  ip_ = tmp[0];
  port_ = strtoul(tmp[1].data(), nullptr, 10);
  LOG(INFO) << "Parse object, " << DOMAIN_SERVER << ": " << domain_server
            << ", " << IP << ": " << ip_ << ", " << PORT << ": " << port_;

  if (!doc.HasMember(LISTEN_QUEUE_LEN) || !doc[LISTEN_QUEUE_LEN].IsUint()) {
    LOG(ERROR) << "Invaild key: " << LISTEN_QUEUE_LEN;
    return make_error_code(Errc::kInvaildKey);
  }
  listen_queue_len_ = doc[LISTEN_QUEUE_LEN].GetUint();
  LOG(INFO) << "Parse object, " << LISTEN_QUEUE_LEN << ": "
            << listen_queue_len_;

  if (!doc.HasMember(RD_BUFFER_LEN) || !doc[RD_BUFFER_LEN].IsUint()) {
    LOG(ERROR) << "Invaild key: " << RD_BUFFER_LEN;
    return make_error_code(Errc::kInvaildKey);
  }
  rd_buffer_len_ = doc[RD_BUFFER_LEN].GetUint();
  LOG(INFO) << "Parse object, " << RD_BUFFER_LEN << ": " << rd_buffer_len_;

  if (!doc.HasMember(WT_BUFFER_LEN) || !doc[WT_BUFFER_LEN].IsUint()) {
    LOG(ERROR) << "Invaild key: " << WT_BUFFER_LEN;
    return make_error_code(Errc::kInvaildKey);
  }
  wt_buffer_len_ = doc[WT_BUFFER_LEN].GetUint();
  LOG(INFO) << "Parse object, " << WT_BUFFER_LEN << ": " << wt_buffer_len_;

  return ec;
}
