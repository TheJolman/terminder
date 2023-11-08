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
    if (month < 1 || month > 12) return false;
    if (year < 0 || year > 99) return false;
    
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    // Adjust for leap years
    if (month == 2 && (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))) {
      daysInMonth[1] = 29;
    }
    
    if (day < 1 || day > daysInMonth[month - 1]) return false;
    
    return true;
  }

public:
  
  bool parseDate(std::string dateStr) {
    std::regex datePattern(R"(^\s*(\d{1,2})/(\d{1,2})/(\d{2})\s*$)");
    std::smatch matches;

    if (std::regex_search(dateStr, matches, datePattern)) {
      if (matches.size() == 4) { // matches[0] is the full match, 1-3 are the groups
        month = std::stoi(matches[1].str());
        day = std::stoi(matches[2].str());
        year = std::stoi(matches[3].str());

        // check if the parsed values constitute a valid date 
        if (isValidDate()) {
          return true;
        }
      }
    }
    return false;
  }

  friend std::ostream& operator<<(std::ostream& os, const Date& d) {
    os << d.month << "/" << d.day << "/" << d.year;
    return os;
  }
  
};
