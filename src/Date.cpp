/**
 * @file Date.cpp
 * @brief Implementation of the Date class
 * @author Joshua Holman
 *
 * @see Date.hpp for interface documentation
 */

#include "Date.hpp"
#include <chrono>
#include <sstream>

std::string Date::toString(const Date &date) noexcept { return std::format("{:%b %d %Y}", date.date); }

std::expected<Date, std::string> Date::fromString(const std::string &dateString) {
  constexpr auto fmt = "%m/%d/%y";
  std::chrono::year_month_day ymd;
  std::istringstream ss(dateString);
  ss >> std::chrono::parse(fmt, ymd);
  if (ss.fail()) {
    return std::unexpected("Failed to parse date: '" + dateString + "' (expected format: mm/dd)");
  }

  return Date(ymd);
}
