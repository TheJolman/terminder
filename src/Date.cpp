/**
 * @file Date.cpp
 * @brief Implementation of the Date class
 * @author Joshua Holman
 *
 * @see Date.hpp for interface documentation
 */

#include "Date.hpp"
#include <chrono>
#include <iomanip>
#include <sstream>

const std::string Date::inputFormatStr = "%m/%d";
const std::string Date::outputFormatStr = "%b %d";

std::string Date::toString(const Date &date) noexcept {
  std::time_t time = std::chrono::system_clock::to_time_t(date.date);
  std::tm *tm = std::localtime(&time);
  std::ostringstream oss;
  oss << std::put_time(tm, outputFormatStr.c_str());
  return oss.str();
}

std::expected<std::chrono::system_clock::time_point, std::string>
Date::parseDate(const std::string &dateString) {
  std::tm tm = {};
  std::istringstream ss(dateString);
  ss >> std::get_time(&tm, inputFormatStr.c_str());

  if (ss.fail()) {
    return std::unexpected("Failed to parse date: '" + dateString +
                           "' (expected format: mm/dd)");
  }

  return std::chrono::system_clock::from_time_t(std::mktime(&tm));
}

std::expected<Date, std::string> Date::fromString(const std::string &dateStr) {
  auto result = parseDate(dateStr);
  if (!result) {
    return std::unexpected(result.error());
  }

  Date date;
  date.date = result.value();
  return date;
}
