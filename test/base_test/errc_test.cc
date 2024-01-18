#include "base/errc.hpp"
#include <gtest/gtest.h>

using namespace std;
using namespace cqss::base::errc;

TEST(ErrcTest, NoErr) {
  // Test for kNoErr
  std::error_code ec = NO_ERR;
  EXPECT_FALSE(ec);  // Expecting no error
  EXPECT_EQ(ErrMsg.at(Errc::kNoErr), ec.message());
}

TEST(ErrcTest, CustomError) {
  // Test for a custom error
  std::error_code customError = ERR(Errc::kErr);
  EXPECT_TRUE(customError);  // Expecting an error
  EXPECT_EQ(ErrMsg.at(Errc::kErr), customError.message());
}

TEST(ErrcTest, UnknownError) {
  // Test for an unknown error
  std::error_code unknownError =
      ERR(static_cast<Errc>(42));  // Assuming 42 is an unknown error code
  EXPECT_TRUE(unknownError);       // Expecting an error
  EXPECT_EQ("Unknown error", unknownError.message());
}

