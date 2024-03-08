/*
 * Project: task
 * File: Task.h
 * Description: abstracts tasks into an object
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
#include <string>
#include "Date.hpp"
#include <sstream>
#include <iostream>

class Task {
private:

  std::string name;
  Date dueDate; 
  bool completion;
  Date currentDate;

public:

  Task(std::string name, Date dueDate)
    : name(name), dueDate(dueDate), completion(false), currentDate(Date()) {
}
  Task(std::string name, Date dueDate, bool completion, Date currentDate)
    : name(name), dueDate(dueDate), completion(completion), currentDate(currentDate) {}

  void markComplete() { completion = true; }

  bool getCompletion() const { return completion; }
  
  std::string getName() const { return name; }

  friend std::ostream& operator<<(std::ostream& os, const Task& t) {
    std::string status = (t.completion ? "Completed" : "Incomplete");
    os << t.name;
    if (t.dueDate.toString() != t.currentDate.toString()) { // for when no due date is provided
      os << "\t" << t.dueDate;
    } else {
      os << "\t\t";
    }

    if (!t.completion && t.isOverdue()) {
      os << "\t\033[31m" << status << "\033[0m"; // red if overdue
    } else {
      os << "\t\033[32m" << status << "\033[0m"; // green if not overdue
    }

    return os;
  }

  std::string serialize() const {
    std::ostringstream oss;
    oss << name << ',' << dueDate << ',' << completion << currentDate;
    return oss.str();
  }

  static Task deserialize(const std::string &data) {
    std::istringstream iss(data);
    std::string token;
    std::getline(iss, token, ',');
    std::string name = token;
    std::getline(iss, token, ',');
    Date dueDate(token);
    std::getline(iss, token, ',');
    bool completion = (token == "1");
    std::getline(iss, token, ',');
    Date currentDate(token);
    return Task(name, dueDate, completion, currentDate);
  }

  // for displaying overdue tasks in red
  bool isOverdue() const {
    return dueDate < currentDate;
  }
};
