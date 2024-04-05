#include "cqss/cmn/errc/errc.hpp"

#include <gtest/gtest.h>


using namespace std;
using namespace cqss::cmn::err;

TEST(ErrcTest, NoErr) {
  // Test no error
  std::error_code ec;
  EXPECT_FALSE(ec);  // Expecting no error
}

TEST(ErrcTest, SystemError) {
  // Test for a system error
  std::error_code customError = std::make_error_code(std::errc::io_error);
  EXPECT_TRUE(customError);  // Expecting an error
  EXPECT_EQ("Input/output error", customError.message());
}

TEST(ErrcTest, CqssCustomError) {
  // Test for a custom error
  std::error_code customError = std::make_error_code(Errc::kInvaildKey);
  EXPECT_TRUE(customError);  // Expecting an error
  EXPECT_EQ("Invaild Key", customError.message());
}

TEST(ErrcTest, CqssUnknownError) {
  // Test for an unknown error
  std::error_code unknownError =
      std::make_error_code((static_cast<Errc>(INTMAX_MAX)));
  EXPECT_TRUE(unknownError);  // Expecting an error
  EXPECT_EQ("Unknown error", unknownError.message());
}
