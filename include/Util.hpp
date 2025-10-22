#pragma once

#include <format>
#include <print>

namespace util {
template <typename... Args> static void error(std::format_string<Args...> fmt, Args &&...args) {
  std::println(stderr, "[ERROR] {}", std::format(fmt, std::forward<Args>(args)...));
}

} // namespace util
