/**
 * @file TaskList.h
 * @brief Interface for the TaskList class
 * @author Joshua Holman
 * @contact jolman@duck.com
 *
 * @copyright Copyright (c) 2024 Joshua Holman
 *
 * This source code is licensed under the MIT License.
 * See the LICENSE file in the project root for full license information.
 */

#pragma once

#include "Task.hpp"
#include <cereal/access.hpp>
#include <cereal/types/vector.hpp>
#include <expected>
#include <filesystem>
#include <optional>
#include <vector>

class TaskList {
public:
  std::expected<void, std::string> addTask(const std::string &taskName,
                                           const std::string &dueDate = "");
  void removeTask(const std::string &) noexcept;
  void completeTask(const std::string &) noexcept;
  void removeCompletedTasks() noexcept;
  void removeAllTasks();
  std::optional<std::vector<Task>> getList() const noexcept;

  std::expected<void, std::string> saveToFile();
  std::expected<void, std::string> loadFromFile();

  void prettyPrint();

private:
  std::vector<Task> list;
  std::filesystem::path dataFilePath;

  friend class cereal::access;

  template <class Archive> void serialize(Archive &ar) { ar(list); }

  std::filesystem::path getSaveLocation();
  void setSaveLocation();
};
