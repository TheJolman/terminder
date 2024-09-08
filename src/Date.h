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

#include <chrono>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <cereal/access.hpp>

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
   * @brief Constructs a date object with the time the object was created
   */
  Date() noexcept : date(std::chrono::system_clock::now()) {}

  /**
   * @brief Constructs a date object with provided date
   * @param dateStr date provided as a string
   */
  Date(const std::string& dateStr) : date(parseDate(dateStr)) {}

  /**
   * @brief Compares two date objects. Used for sorting 
   */
  bool operator<(const Date& other) const {
      return date < other.date;
  }

  /**
   * @brief Overloaded subraction to get the diff between two dates in hours
   */
  std::chrono::hours operator-(const Date& other) const {
    return std::chrono::duration_cast<std::chrono::hours>(date-other.date);
  }

  /**
   * @brief Converts Date.futureDate to a string
   */
  static std::string toString(const Date& date) noexcept;

  /**
   * @brief Used for printing date objects with std::cout
   */
  friend std::ostream& operator<<(std::ostream& os, const Date& d) noexcept;

private:

  std::chrono::system_clock::time_point date;
  static const std::string inputFormatStr;
  static const std::string outputFormatStr;

  friend class cereal::access;

  template<class Archive>
  void serialize(Archive& archive) {
    archive(date);
  }

  /**
   * @brief Used in the constructor to convert date strings to std::chrono::system_clock::time_point
   * @param dateString User provided date string in the form of mm/dd
   */
  std::chrono::system_clock::time_point parseDate(const std::string& dateString);
  
};
