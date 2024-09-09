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

#include <cereal/access.hpp>
#include <cereal/types/list.hpp>
#include "Task.h"
#include <list>
#include <filesystem>
#include <optional>

class TaskList {
public:

  void addTask(const std::string& taskName, std::optional<std::string> dueDate = std::nullopt);
  void removeTask(const std::string&) noexcept;
  void completeTask(const std::string&) noexcept;
  void removeCompletedTasks() noexcept;
  void removeAllTasks();
  std::optional<std::list<Task>> getList() const noexcept;

  void saveToFile();
  void loadFromFile();


private:
  std::list<Task> list;
  std::filesystem::path dataFilePath;

  friend class cereal::access;

  template<class Archive>
  void serialize(Archive& ar) {
    ar(list);
  }

  std::filesystem::path getSaveLocation();
  void setSaveLocation();
};
