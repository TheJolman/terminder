/**
 * @file TaskList.cpp
 * @brief Implementation for the TaskList class
 * @author Joshua Holman
 * @contact jolman@duck.com
 * 
 * @copyright Copyright (c) 2024 Joshua Holman
 * 
 * This source code is licensed under the MIT License.
 * See the LICENSE file in the project root for full license information.
 */

#include "TaskList.h"
#include <filesystem>
#include <cereal/archives/json.hpp>
#include <fstream>
#include <optional>

void TaskList::addTask(const std::string& taskName, std::optional<std::string> dueDate) {
  if (dueDate.has_value()) {
    /*std::unique_ptr<Task> taskPtr(new Task(taskName, dueDate.value()));*/
    list.emplace_front(Task(taskName, dueDate.value()));
  } else {
    /*std::unique_ptr<Task> taskPtr(new Task(taskName));*/
    list.emplace_front(Task(taskName));

  }

  /*list.emplace_front(taskPtr);*/
}

void TaskList::removeTask(const std::string& taskName) noexcept {
  bool found = false;
  for (auto it = list.begin(); it != list.end(); it++) {
    if (taskName == it->getName()) {
      list.erase(it);
      found = true;
    }
  }
  if (!found) {
    std::cout << "No task with the name '" << taskName << "' found.\n";
  }
}

void TaskList::completeTask(const std::string& taskName) noexcept {
  bool found = false;
  for (Task& task : list) {
    if (taskName == task.getName()) {
      task.markComplete();
    }
  }
  if (!found) {
    std::cout << "No task with the name '" << taskName << "' found.\n";
  }
}

void TaskList::removeCompletedTasks() noexcept {
  for (auto it = list.begin(); it != list.end(); it++) {
    if (it->isComplete()) {
      list.erase(it);
    }
  }

}


std::optional<std::list<Task>> TaskList::getList() const noexcept {
  if (list.empty()) {
    return std::nullopt;
  }
  return list;
}


void TaskList::saveToFile() {
  std::string fileName = "data.json";
  std::filesystem::path appDataDir = getSaveLocation();
  std::filesystem::create_directory(appDataDir);
  dataFilePath = appDataDir / fileName;

  std::ofstream outFile(dataFilePath);
  cereal::JSONOutputArchive oarchive(outFile);
  oarchive(*this); // is this correct?

}

void TaskList::loadFromFile() {
  
  std::ifstream inFile(dataFilePath);
  cereal::JSONInputArchive iarchive(inFile);

  iarchive(*this);
}


std::filesystem::path TaskList::getSaveLocation() {
  std::string appName = "terminder";
  std::filesystem::path dataHome;

  // Check if XDG_DATA_HOME is set
  if (const char* envDataHome = std::getenv("XDG_DATA_HOME")) {
    dataHome = envDataHome;
  } else {
    const char* home = std::getenv("HOME");
    if (home == nullptr) {
      throw std::runtime_error("HOME environment variable not set");
    }

    dataHome = std::filesystem::path(home) / ".local" / "share";
  }

  return dataHome / appName;
}
