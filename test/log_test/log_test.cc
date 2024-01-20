#include "cqss/log/log.hpp"

#include <gtest/gtest.h>

using namespace cqss::log;

TEST(LogTest, LogDebugTest) {
    testing::internal::CaptureStdout();
    LOG_DEBUG("This is a message with arguments: {}", "debug");
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find("[DEBUG]") != std::string::npos);
    EXPECT_TRUE(output.find("This is a message with arguments: debug") != std::string::npos);
}

TEST(LogTest, LogInfoTest) {
    testing::internal::CaptureStdout();
    LOG_INFO("This is a message with arguments: {}", "info");
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find("[INFO]") != std::string::npos);
    EXPECT_TRUE(output.find("This is a message with arguments: info") != std::string::npos);
}

TEST(LogTest, LogWarnTest) {
    testing::internal::CaptureStdout();
    LOG_WARN("This is a message with arguments: {}", "warn");
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find("[WARN]") != std::string::npos);
    EXPECT_TRUE(output.find("This is a message with arguments: warn") != std::string::npos);
}

TEST(LogTest, LogErrorTest) {
    testing::internal::CaptureStdout();
    LOG_ERROR("This is a message with arguments: {}", "error");
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find("[ERROR]") != std::string::npos);
    EXPECT_TRUE(output.find("This is a message with arguments: error") != std::string::npos);
}

TEST(LogTest, LogFatalTest) {
    testing::internal::CaptureStdout();
    LOG_FATAL("This is a message with arguments: {}", "fatal");
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find("[FATAL]") != std::string::npos);
    EXPECT_TRUE(output.find("This is a message with arguments: fatal") != std::string::npos);
}
