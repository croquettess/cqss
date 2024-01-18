/**
 * @file singleton.hpp
 * @author croquettess
 * @date 2024-01-01
 *
 * @brief 单例模式
 *
 */

namespace cqss {
namespace design {

template <typename T>
class Singleton {
 public:
  static T &GetInstance() {
    static T instance;
    return instance;
  }

  Singleton(const T &) = delete;
  T operator=(const T &) = delete;

 protected:
  Singleton() noexcept = default;
  virtual ~Singleton() noexcept = default;
};



}  // namespace design
}  // namespace cqss
