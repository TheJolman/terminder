/*
 * Project: task
 * File: Date.h
 * Description: handles dates
 * Copyright (C) 2024 Joshua Holman
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Joshua Holman
 * Contact: jolman@duck.com
 */
#pragma once

#include <chrono>
#include <sstream>
#include <iomanip>
#include <iostream>

class Date {
private:
  std::chrono::system_clock::time_point date;
  std::chrono::system_clock::time_point currentDate;

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

  
public:
  // default constructor creates a date with the current date
  Date() : currentDate(std::chrono::system_clock::now()) {
    auto now = std::chrono::system_clock::now();
    time_t tt = std::chrono::system_clock::to_time_t(now);
    tm local_tm = *localtime(&tt);
    date = std::chrono::system_clock::from_time_t(mktime(&local_tm));
}

  // creating a date with mm/dd/yyyy
  Date(const std::string& dateStr) : currentDate(std::chrono::system_clock::now()) {
      std::istringstream ss(dateStr);
      int month, day, year;
      char delimiter;
      if (ss >> month >> delimiter >> day >> delimiter >> year) {
          if (delimiter != '/' || month < 1 || month > 12 || day < 1 || day > 31 || year < 0 || year > 99) {
              throw std::invalid_argument("Invalid date format: does not match mm/dd/yy.");
          }
          std::tm tm = {};
          tm.tm_year = year + 100; // Adjust year to be in the range 1900-1999
          tm.tm_mon = month - 1;   // Month is 0-based in std::tm
          tm.tm_mday = day;
          if (!isValidDate(tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900)) {
              throw std::invalid_argument("Invalid date: values are not valid.");
          }
          date = std::chrono::system_clock::from_time_t(mktime(&tm));
      } else {
          throw std::invalid_argument("Invalid date format: does not match mm/dd/yy.");
      }
  }
  std::string toString() const {
      std::time_t tt = std::chrono::system_clock::to_time_t(date);
      tm local_tm = *localtime(&tt);
      char buffer[11];
      strftime(buffer, sizeof(buffer), "%m/%d/%y", &local_tm);
      return std::string(buffer);
  }

  bool operator<(const Date& other) const {
      return date < other.date;
  }

  friend std::ostream& operator<<(std::ostream& os, const Date& d) {
    std::time_t tt = std::chrono::system_clock::to_time_t(d.date);
    tm local_tm = *localtime(&tt);
    os << std::put_time(&local_tm, "%m/%d/%y");
    return os;
}
};
