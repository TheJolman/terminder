// Date.h
#pragma once
#include <iostream>
#include <string>
#include <regex>

class Date {
private:
  
  int month;
  int day;
  int year;

  bool isValidDate() {
    int tempyear = year + 2000;
    if (month < 1 || month > 12) return false;
    if (tempyear < 0 || tempyear > 99) return false;
    if (tempyear < 2000 || tempyear > 2099) return false; 
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    // Adjust for leap years
    if (month == 2 && (tempyear % 4 == 0 && (tempyear % 100 != 0 || tempyear % 400 == 0))) {
      daysInMonth[1] = 29;
    }
    
    return day <= daysInMonth[month - 1];
  }

public:

  Date() : month(1), day(1), year(00) {}
  Date(const Date &other) {
    month = other.month;
    day = other.day;
    year = other.year;
  }
  Date(const std::string& dateStr) {
        std::regex datePattern(R"(^\s*(\d{1,2})/(\d{1,2})/(\d{2})\s*$)");
        std::smatch matches;

        if (std::regex_match(dateStr, matches, datePattern)) {
            if (matches.size() == 4) { // matches[0] is the full match, 1-3 are the groups
                month = std::stoi(matches[1].str());
                day = std::stoi(matches[2].str());
                year = std::stoi(matches[3].str()); // Assuming year input is yy format for 2000s

                if (!isValidDate()) {
                    throw std::invalid_argument("Invalid date: values are not valid.");
                }
            }
        } else {
            throw std::invalid_argument("Invalid date format: does not match mm/dd/yy.");
        }
    }

  Date& operator=(const Date &other) {
    if (this != &other) {
      this->month = other.month;
      this->day = other.day;
      this->year = other.year;
    }
    return *this;
  }

  friend std::ostream& operator<<(std::ostream& os, const Date& d) {
    os << d.month << "/" << d.day << "/" << d.year;
    return os;
  }
  
};
