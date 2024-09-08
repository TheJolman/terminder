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
#include "Task.h"
#include <list>
#include <filesystem>

class TaskList {
public:

  void addTask(const std::string&);
  void removeTask();
  void completeTask(const std::string&);
  void removeCompletedTasks();

  void saveToFile();
  void loadFromFile();


private:
  std::list<Task> list;

  friend class cereal::access;

  template<class Archive>
  void serialize(Archive& archive) {
    archive();
  }

  std::filesystem::path getSaveLocation();
};
