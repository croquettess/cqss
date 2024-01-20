#include "cmn/design/singleton.hpp"

#include <gtest/gtest.h>

using namespace cqss::cmn::design;

TEST(DesignTest, ISingletonTest) {
  class Singleton : public ISingleton<Singleton> {
    friend class ISingleton<Singleton>;
   private:
    Singleton() = default;
    ~Singleton() = default;
  };

  Singleton &obj_1 = Singleton::Instance();
  Singleton &obj_2 = Singleton::Instance();
  EXPECT_EQ(&obj_1, &obj_2);
}
