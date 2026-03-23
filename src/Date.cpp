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

const std::string Date::inputFormatStr = "%m/%d";
const std::string Date::outputFormatStr = "%b %d";

std::string Date::toString() const noexcept { return std::format("{:%b %d}", date); }

std::expected<std::chrono::year_month_day, std::string>
Date::fromString(const std::string &dateString) {
  // std::tm tm = {};
  std::chrono::year_month_day ymd;
  std::istringstream ss(dateString);
  ss >> std::chrono::parse(inputFormatStr, ymd);
  if (ss.fail()) {
    return std::unexpected("Failed to parse date: '" + dateString + "' (expected format: mm/dd)");
  }

  return ymd;
}
