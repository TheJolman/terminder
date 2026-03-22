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
#include <tabulate/font_style.hpp>
#include <tabulate/table.hpp>

std::expected<void, std::string> TaskList::addTask(const std::string &taskName,
                                                   std::optional<std::string> dueDate) {
  if (dueDate.has_value()) {
    auto taskResult = Task::create(taskName, dueDate.value());
    if (!taskResult) {
      return std::unexpected(taskResult.error());
    }
    list.emplace_front(taskResult.value());
  } else {
    list.emplace_front(Task(taskName));
  }
  return {};
}

void TaskList::removeTask(const std::string &taskName) noexcept {
  auto it = std::find_if(list.begin(), list.end(),
                         [&taskName](const Task &task) { return task.getName() == taskName; });
  if (it != list.end()) {
    list.erase(it);
  }
}

void TaskList::completeTask(const std::string &taskName) noexcept {
  auto it = std::find_if(list.begin(), list.end(), [&taskName](const Task &task) {
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
    return std::unexpected(
        std::format("unable to open file for writing: {}", dataFilePath.string()));
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
    return std::unexpected(
        std::format("unable to open file for reading: {}", dataFilePath.string()));
  }
  try {
    cereal::JSONInputArchive archive(inFile);
    archive(*this);
  } catch (const cereal::Exception &e) {
    return std::unexpected(std::format("error parsing JSON file: {}", e.what()));
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

void TaskList::prettyPrint() {
  if (list.empty()) {
    std::println("No tasks to display.");
    return;
  }
  using namespace tabulate;
  Table task_table;
  int idx = 1;
  task_table.add_row({"#", "Task", "Status", "Due Date"});
  task_table[0]
      .format()
      .font_style({FontStyle::bold})
      .font_background_color(Color::white)
      .font_color(Color::grey);
  for (const auto &t : this->list) {
    auto dateStr = t.getDueDate()
                       .transform([](const auto &date) { return Date::toString(date); })
                       .value_or("");
    auto completion = t.isComplete() ? "DONE" : "TODO";
    task_table.add_row({std::to_string(idx++), t.getName(), completion, dateStr});
  }
  // task_table.format();
  std::cout << task_table << std::endl;
}
