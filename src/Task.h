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
#include "Date.h"
#include <sstream>

class Task {
private:

  std::string name;
  Date dueDate; 
  bool completion;

public:
  Task(std::string name, Date dueDate) : name(name), dueDate(dueDate), completion(false) {}
  Task(std::string name, Date dueDate, bool completion) : name(name), dueDate(dueDate), completion(completion) {}

  void markComplete() { completion = true; }

  bool getCompletion() const { return completion; }
  
  std::string getName() const { return name; }

  friend std::ostream& operator<<(std::ostream& os, const Task& t) {
    std::string status = (t.completion ? "Completed" : "Incomplete");
    os << t.name << "\t" << t.dueDate << "\t" << status;
    return os;
  }

  std::string serialize() const {
    std::ostringstream oss;
    oss << name << ',' << dueDate << ',' << completion;
    return oss.str();
  }

  static Task deserialize(const std::string &data) {
    std::istringstream iss(data);
    std::string token;
    std::getline(iss, token, ',');
    std::string name = token;
    std::getline(iss, token, ',');
    Date date;
    try {
      Date tryDate(token);
      date = tryDate;
    }
    catch (const std::invalid_argument &e) {
      std::cerr << "Error: " << e.what() << std::endl;
    }
    std::getline(iss, token, ',');
    bool completion;
    if (token == "1")
      completion = true;
    else
      completion = false;
    return Task(name, date, completion);
  }
};
