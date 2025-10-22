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
#include <cereal/types/optional.hpp>
#include <expected>
#include <format>
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
  Task() : name(""), completionStatus(false), dueDate(std::nullopt) {}

  // Constructor with name
  Task(const std::string &name) : name(name), completionStatus(false), dueDate(std::nullopt) {}

  // Constructor with name and due date
  // Task(std::string name, std::string dueDateStr)
  //     : name(name), completionStatus(false), dueDate(Date(dueDateStr)) {
  //   Date initialDate;
  //   timeUntilDue = dueDate - initialDate;
  // }

  /**
   * @brief Creates a Task with proper error handling for date parsing
   * @param name Task name
   * @param dueDateStr Due date string
   * @return Expected Task or error message
   */
  static std::expected<Task, std::string> create(const std::string &name,
                                                 const std::string &dueDateStr) {
    auto dateResult = Date::fromString(dueDateStr);
    if (!dateResult) {
      return std::unexpected(dateResult.error());
    }

    Task task;
    task.name = name;
    task.completionStatus = false;
    task.dueDate = dateResult.value();

    Date initialDate;

    return task;
  }

  void markComplete() { completionStatus = true; }

  std::string getName() const { return name; }
  bool isComplete() const { return completionStatus; }
  std::optional<Date> getDueDate() const { return dueDate; }

private:
  friend class cereal::access;

  template <class Archive> void serialize(Archive &ar) {
    ar(CEREAL_NVP(name), CEREAL_NVP(completionStatus), CEREAL_NVP(dueDate));
  }

  std::string name;
  bool completionStatus;
  std::optional<Date> dueDate;
};

template <> struct std::formatter<Task> {
  constexpr auto parse(std::format_parse_context &ctx) { return ctx.begin(); }

  auto format(const Task &t, std::format_context &ctx) const {
    return std::format_to(ctx.out(), "{}", t.getName());
  }
};
