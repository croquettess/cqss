/**
 * @file errc.h
 * @author croquettess
 * @date 2024-01-19
 *
 * @brief error code wrapper
 */

#ifndef CQSS_CMN_ERR_ERR_HPP_
#define CQSS_CMN_ERR_ERR_HPP_

#include <system_error>
#include <unordered_map>

namespace cqss {
namespace cmn {
namespace err {
enum class Errc : int {
  kInvaildValue,
  kInvaildCfg,
  kInvaildKey,
  kInvaildProtocol,
  kInvaildModel,
  kInvaildAddress,
};

class ErrCategory : public std::error_category {
 public:
  constexpr ErrCategory() = default;
  ErrCategory(ErrCategory &) = delete;
  ErrCategory &operator=(ErrCategory &) = delete;

  inline static ErrCategory &Instance() {
    static ErrCategory instance;
    return instance;
  }

  inline const char *name() const noexcept override {
    return "cqss error category";
  }

  std::string message(int ec) const override {
    switch (static_cast<Errc>(ec)) {
      case Errc::kInvaildValue:
        return "Invaild Value";
      case Errc::kInvaildCfg:
        return "Invaild Cfg";
      case Errc::kInvaildKey:
        return "Invaild Key";
      case Errc::kInvaildProtocol:
        return "Invaild Protocol";
      case Errc::kInvaildModel:
        return "Invaild Model";
      case Errc::kInvaildAddress:
        return "Invaild Address";
      default:
        return "Unknown error";
    }
  }
};

}  // namespace err
}  // namespace cmn
}  // namespace cqss

namespace std {
template <>
struct is_error_code_enum<cqss::cmn::err::Errc> : true_type {};

inline error_code make_error_code(cqss::cmn::err::Errc ec) {
  return {static_cast<int>(ec), cqss::cmn::err::ErrCategory::Instance()};
}

}  // namespace std

#endif  // CQSS_CMN_ERR_ERR_HPP_
