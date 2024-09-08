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
#include <string>
#include "Date.h"
#include <sstream>
#include <iostream>

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
public:

  Task(std::string name) : name(name), completionStatus(false) {
    timeUntilDue = std::chrono::hours(0);
  }

  Task(std::string name, Date dueDateStr) : name(name), completionStatus(false), dueDate(Date(dueDateStr)) {
    Date initialDate;
    timeUntilDue = dueDate - initialDate;
  }

  void markComplete() { completionStatus = true; }

  std::string getName() { return this->name; }
  bool isComplete() { return this->completionStatus; }
  Date getDueDate() { return this->dueDate; }
  int getTimeUntilDue() { return this->timeUntilDue.count(); } // maybe needs to be long long

private:

  std::string name;
  bool completionStatus;
  Date dueDate; 
  std::chrono::hours timeUntilDue;

};
