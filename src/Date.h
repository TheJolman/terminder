/**
 * @file Date.h
 * @brief Handles two dates
 * @author Joshua Holman
 * 
 * @copyright Copyright (c) 2024 Joshua Holman
 * 
 * This source code is licensed under the MIT License.
 * See the LICENSE file in the project root for full license information.
 */

#pragma once

#include <chrono>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <sstream>

/**
 * @brief A class that uses chrono to handle due dates
 * @author Joshua Holman
 * @date 2024-09-07
 *
 * This class provides string input and output for dates. It handles two dates:
 * the current date, which is initialized at object creation, and a future date
 * provided by the user. Includes methods for validating dates and calculating
 * the difference between the two.
 */
class Date {
public:
  /**
   * @brief Constructs a date object with the same future time as current
   */
  Date() noexcept : currentDateTime(std::chrono::system_clock::now()), 
              futureDateTime(std::chrono::system_clock::now()) {}

  /**
   * @brief Constructs a date object with provided future date
   * @param dateStr Future date provided as a string
   */
  Date(const std::string& dateStr) : currentDateTime(std::chrono::system_clock::now()),
              futureDateTime(parseDate(dateStr)) {}

  /**
   * @brief Compares two date objects. Used for sorting 
   */
  bool operator<(const Date& other) const {
      return futureDateTime < other.futureDateTime;
  }

  /**
   * @brief Used for printing date objects with std::cout
   */
  friend std::ostream& operator<<(std::ostream& os, const Date& d) {
    std::time_t tt = std::chrono::system_clock::to_time_t(d.futureDateTime);
    tm local_tm = *localtime(&tt);
    os << std::put_time(&local_tm, outputFormatStr);
    return os;
}

private:
  std::chrono::system_clock::time_point futureDateTime;
  std::chrono::system_clock::time_point currentDateTime;
  const std::string inputFormatStr = "%m/%d";
  const std::string outputFormatStr = "%m/%d";


  bool isValidDate(int day, int month, int year) {
    if(year < 0 || year > 9999 || month < 1 || month > 12)
        return false;
    if(day < 1 || day > 31)
        return false;
    if(month == 2) {
        bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if(day > 29 || (day == 29 && !isLeap))
            return false;
    }
    if(day == 31 && (month == 4 || month == 6 || month == 9 || month == 11))
        return false;
    return true;
  }

  std::chrono::system_clock::time_point parseDate(const std::string& dateString) {
    std::tm tm = {};
    std::istringstream ss(dateString);
    ss >> std::get_time(&tm, inputFormatStr.c_str());

    if (ss.fail()) {
      throw std::runtime_error("Date parse failed");
    }

    return std::chrono::system_clock::from_time_t(std::mktime(&tm));
  }

  
};
