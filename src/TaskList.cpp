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

#include "TaskList.hpp"
#include <cereal/archives/json.hpp>
#include <filesystem>
#include <fstream>
#include <optional>
#include <print>

void TaskList::addTask(const std::string &taskName,
                       std::optional<std::string> dueDate) {
  if (dueDate.has_value()) {
    list.emplace_front(Task(taskName, dueDate.value()));
  } else {
    list.emplace_front(Task(taskName));
  }
}

void TaskList::removeTask(const std::string &taskName) noexcept {
  auto it =
      std::find_if(list.begin(), list.end(), [&taskName](const Task &task) {
        return task.getName() == taskName;
      });
  if (it != list.end()) {
    list.erase(it);
  }
}

void TaskList::completeTask(const std::string &taskName) noexcept {
  auto it =
      std::find_if(list.begin(), list.end(), [&taskName](const Task &task) {
        return task.getName() == taskName && !task.isComplete();
      });
  if (it != list.end()) {
    it->markComplete();
  }
}

void TaskList::removeCompletedTasks() noexcept {
  list.remove_if([](const Task &task) { return task.isComplete(); });
}

void TaskList::removeAllTasks() { list.clear(); }

std::optional<std::list<Task>> TaskList::getList() const noexcept {
  if (list.empty()) {
    return std::nullopt;
  }
  return list;
}

std::expected<void, std::string> TaskList::saveToFile() {
  setSaveLocation();

  std::ofstream outFile(dataFilePath);
  if (!outFile) {
    return std::unexpected("unable to open file for writing:" +
                           dataFilePath.string());
  }
  cereal::JSONOutputArchive oarchive(outFile);
  oarchive(*this);
  return {};
}

std::expected<void, std::string> TaskList::loadFromFile() {
  setSaveLocation();

  if (!std::filesystem::exists(dataFilePath)) {
    // if the file doesn't exist yet, just return without throwing an exception
    return {};
  }

  std::ifstream inFile(dataFilePath);
  if (!inFile) {
    return std::unexpected("unable to open file for reading: " +
                           dataFilePath.string());
  }
  try {
    cereal::JSONInputArchive archive(inFile);
    archive(*this);
  } catch (const cereal::Exception &e) {
    return std::unexpected("Error parsing JSON file: " + std::string(e.what()));
  }

  return {};
}

std::filesystem::path TaskList::getSaveLocation() {
  std::string appName = "terminder";
  std::filesystem::path dataHome;

  // Check if XDG_DATA_HOME is set
  if (const char *envDataHome = std::getenv("XDG_DATA_HOME")) {
    dataHome = envDataHome;
  } else {
    const char *home = std::getenv("HOME");
    if (!home) {
      std::println(stderr, "[ERROR] HOME environment variable is not set. exiting...");
      std::exit(1);
    }
    dataHome = std::filesystem::path(home) / ".local" / "share";
  }

  return dataHome / appName;
}

void TaskList::setSaveLocation() {
  std::string fileName = "data.json";
  std::filesystem::path appDataDir = getSaveLocation();
  std::filesystem::create_directory(appDataDir);
  dataFilePath = appDataDir / fileName;
}
