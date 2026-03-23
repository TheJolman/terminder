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
#include <expected>
#include <filesystem>
#include <optional>
#include <vector>

class TaskList {
public:
  void addTask(const Task &) noexcept;
  void removeTask(const size_t) noexcept;
  void completeTask(const size_t) noexcept;
  void removeCompletedTasks() noexcept;
  void removeAllTasks();
  std::optional<std::vector<Task>> getList() const noexcept;

  std::expected<void, std::string> saveToFile();
  std::expected<void, std::string> loadFromFile();

  void prettyPrint();
  std::optional<size_t> findTask(const std::string &) const;
  std::optional<Task> getTask(const size_t index) const;

private:
  std::vector<Task> list;
  std::filesystem::path dataFilePath;

  std::filesystem::path getSaveLocation();
  void setSaveLocation();
};
