#ifndef CQSS_CMN_DESIGN_SINGLETON_HPP_
#define CQSS_CMN_DESIGN_SINGLETON_HPP_

namespace cqss {
namespace cmn {
namespace design {

template <typename T>
class ISingleton {
 public:
  inline static T &Instance() {
    static T instance;
    return instance;
  }

  ISingleton(const T &) = delete;
  T operator=(const T &) = delete;

 protected:
  ISingleton() noexcept = default;
  virtual ~ISingleton() noexcept = default;
};

}  // namespace design
}  // namespace cmn
}  // namespace cqss

#endif  // CQSS_CMN_DESIGN_SINGLETON_HPP_
