/**
 * @file Date.h
 * @brief Interface for the Date class
 * @author Joshua Holman
 * @contact jolman@duck.com
 *
 * @copyright Copyright (c) 2024 Joshua Holman
 *
 * This source code is licensed under the MIT License.
 * See the LICENSE file in the project root for full license information.
 */

#pragma once

#include <cereal/access.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/chrono.hpp>
#include <chrono>
#include <expected>
#include <format>

/**
 * @brief A class that uses std::chrono to handle due dates
 *
 */
class Date {
public:
  Date() = default;
  Date(const std::chrono::year_month_day &ymd) : date(ymd) {}

  static std::string toString(const Date &date) noexcept;
  static std::expected<Date, std::string> fromString(const std::string &dateStr);

private:
  std::chrono::year_month_day date;

  friend class cereal::access;

  template <class Archive> void serialize(Archive &ar) {
    int y;
    unsigned m, d;
    if constexpr (Archive::is_saving::value) {
      y = static_cast<int>(date.year());
      m = static_cast<unsigned>(date.month());
      d = static_cast<unsigned>(date.day());
    }
    ar(cereal::make_nvp("year", y), cereal::make_nvp("month", m), cereal::make_nvp("day", d));
    if constexpr (Archive::is_loading::value) {
      date = std::chrono::year(y) / m / d;
    }
  }
};

template <> struct std::formatter<Date> {
  constexpr auto parse(std::format_parse_context &ctx) { return ctx.begin(); }

  auto format(const Date &d, std::format_context &ctx) const {
    return std::format_to(ctx.out(), "{}", Date::toString(d));
  }
};
