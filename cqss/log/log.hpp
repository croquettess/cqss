#ifndef CQSS_LOG_LOG_HPP_
#define CQSS_LOG_LOG_HPP_

#include <string>
#include <iostream>
#include <format>
#include <chrono>
#include <source_location>

#define LOG_DEBUG(fmt, ...)                                              \
  log(cqss::log::LogLevel::kDebug, fmt, std::source_location::current(), \
      ##__VA_ARGS__)

#define LOG_INFO(fmt, ...)                                              \
  log(cqss::log::LogLevel::kInfo, fmt, std::source_location::current(), \
      ##__VA_ARGS__)

#define LOG_WARN(fmt, ...)                                              \
  log(cqss::log::LogLevel::kWarn, fmt, std::source_location::current(), \
      ##__VA_ARGS__)

#define LOG_ERROR(fmt, ...)                                              \
  log(cqss::log::LogLevel::kError, fmt, std::source_location::current(), \
      ##__VA_ARGS__)

#define LOG_FATAL(fmt, ...)                                              \
  log(cqss::log::LogLevel::kFatal, fmt, std::source_location::current(), \
      ##__VA_ARGS__)

namespace cqss {
namespace log {

enum class LogLevel {
  kDebug,
  kInfo,
  kWarn,
  kError,
  kFatal,
};

template <typename... Args>
void log(LogLevel level, std::string_view fmt,
         const std::source_location &location, Args &&...args) {
  std::string_view levelStr;
  switch (level) {
    case LogLevel::kInfo:
      levelStr = "INFO";
      break;
    case LogLevel::kWarn:
      levelStr = "WARN";
      break;
    case LogLevel::kError:
      levelStr = "ERROR";
      break;
    case LogLevel::kFatal:
      levelStr = "FATAL";
      break;
    case LogLevel::kDebug:
      levelStr = "DEBUG";
      break;
  }

  auto now = std::chrono::system_clock::now();
  auto logEntry = std::format(
      "[{:%Y-%m-%d %H:%M:%S}] [{}] [{}:{} {}]: {}", now, levelStr,
      location.file_name(), location.line(), location.function_name(),
      std::vformat(fmt, std::make_format_args(args...)));
  std::cout << logEntry << std::endl;
}

}  // namespace log
}  // namespace cqss

#endif  // CQSS_LOG_LOG_HPP_
