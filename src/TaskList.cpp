/**
 * @file TaskList.h
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

// TODO
void TaskList::addTask(const std::string& taskName) {

}

void TaskList::removeTask() {

}

void TaskList::completeTask(const std::string& taskName) {

}

void TaskList::removeCompletedTasks() {

}

void TaskList::saveToFile() {

}

void TaskList::loadFromFile() {

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
