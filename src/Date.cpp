/**
 * @file Date.cpp
 * @brief Implementation of the Date class
 * @author Joshua Holman
 *
 * @see Date.h for interface documentation
 */

#include "Date.hpp"
#include <iomanip>
#include <chrono>
#include <sstream>

const std::string Date::inputFormatStr = "%m/%d";
const std::string Date::outputFormatStr = "%b %d";

std::string Date::toString(const Date& date) noexcept {
    std::time_t time = std::chrono::system_clock::to_time_t(date.date);
    std::tm* tm = std::localtime(&time);
    std::ostringstream oss;
    oss << std::put_time(tm, outputFormatStr.c_str());
    return oss.str();
}


std::ostream& operator<<(std::ostream& os, const Date& d) noexcept {
    os << Date::toString(d);
    return os;
}


std::chrono::system_clock::time_point Date::parseDate(const std::string& dateString) {
  std::tm tm = {};
  std::istringstream ss(dateString);
  ss >> std::get_time(&tm, inputFormatStr.c_str());

  if (ss.fail()) {
    throw std::runtime_error("Date parse failed");
  }

  return std::chrono::system_clock::from_time_t(std::mktime(&tm));
}
