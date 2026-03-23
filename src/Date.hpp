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
#include <stdexcept>

/**
 * @brief A class that uses chrono to handle due dates
 *
 * This class provides can parse strings to create dates and create string
 * representations for output. It handles two dates: the current date, which
 * is initialized at object creation, and a future date provided by the user.
 * Includes methods for validating dates and calculating the difference between
 * the two.
 */
class Date {
public:
  /**
   * @brief Constructs a date object with provided date
   * @param dateStr date provided as a string
   * @note This constructor will create a default date if parsing fails
   */
  Date() = default;
  Date(const std::string &dateStr) {
    auto result = fromString(dateStr);
    if (result) {
      date = result.value();
    } else {
      throw std::invalid_argument("invalid date format");
    }
  }
  Date(const std::chrono::year_month_day &ymd) : date(ymd) {}

  std::string toString() const noexcept;

  static std::expected<std::chrono::year_month_day, std::string>
  fromString(const std::string &dateStr);

private:
  std::chrono::year_month_day date;
  static const std::string inputFormatStr;
  static const std::string outputFormatStr;

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
    return std::format_to(ctx.out(), "{}", d.toString());
  }
};
