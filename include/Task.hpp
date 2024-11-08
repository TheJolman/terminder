/**
 * @file Task.h
 * @brief Interface for the Task class
 * @author Joshua Holman
 * @contact jolman@duck.com
 *
 * @copyright Copyright (c) 2024 Joshua Holman
 *
 * This source code is licensed under the MIT License.
 * See the LICENSE file in the project root for full license information.
 */

#pragma once
#include "Date.hpp"
#include <cereal/access.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/string.hpp>
#include <iostream>
#include <sstream>
#include <string>

/**
 * @brief A class abstracts Tasks with a name, due date, and time to complete
 *        and completion status
 *
 * This class provides can parse strings to create dates and create string
 * representations for output. It handles two dates: the current date, which
 * is initialized at object creation, and a future date provided by the user.
 * Includes methods for validating dates and calculating the difference between
 * the two.
 */
class Task {
  // TODO: Get time until due
public:
  // Default constructor
  Task()
      : name(""), completionStatus(false), timeUntilDue(std::chrono::hours(0)) {
  }

  // Constructor with name
  Task(const std::string &name)
      : name(name), completionStatus(false),
        timeUntilDue(std::chrono::hours(0)) {}

  // Constructor with name and due date
  Task(std::string name, std::string dueDateStr)
      : name(name), completionStatus(false), dueDate(Date(dueDateStr)) {
    Date initialDate;
    timeUntilDue = dueDate - initialDate;
  }

  void markComplete() { completionStatus = true; }

  std::string getName() const { return name; }
  bool isComplete() const { return completionStatus; }
  Date getDueDate() const { return dueDate; }
  int getTimeUntilDue() const {
    return timeUntilDue.count();
  } // maybe needs to be long long

  friend std::ostream &operator<<(std::ostream &os, const Task &t) noexcept {
    // TODO: put time until due in terms of days + hours
    os << t.name << "\t" << (t.completionStatus ? "Complete" : "Incomplete")
       << "\t" << t.dueDate;
    return os;
  }

private:
  friend class cereal::access;

  template <class Archive> void serialize(Archive &ar) {
    ar(CEREAL_NVP(name), CEREAL_NVP(completionStatus), CEREAL_NVP(dueDate),
       CEREAL_NVP(timeUntilDue));
  }

  std::string name;
  bool completionStatus;
  Date dueDate;
  std::chrono::hours timeUntilDue;
};
