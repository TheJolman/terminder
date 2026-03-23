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
#include <cereal/types/optional.hpp>
#include <cereal/types/string.hpp>
#include <format>
#include <string>

/**
 * @brief Tasks have a name, completion status, and due date.
 */
class Task {
  // TODO: Get time until due
public:
  // Default constructor
  Task() : name(""), completionStatus(false), dueDate(std::nullopt) {}

  // Constructor with name
  Task(const std::string &n) : name(n), completionStatus(false), dueDate(std::nullopt) {}

  // Constructor with and date
  Task(const std::string &n, const Date &d) : name(n), completionStatus(false), dueDate(d) {}

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
