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
#include <stdexcept>
#include <tabulate/font_style.hpp>
#include <tabulate/table.hpp>

std::expected<void, std::string> TaskList::addTask(const std::string &taskName,
                                                   const std::string &dueDate) {
  if (dueDate.empty()) {
    list.emplace_back(Task(taskName));
  } else {
    auto taskResult = Task::create(taskName, dueDate);
    if (!taskResult) {
      return std::unexpected(taskResult.error());
    }
    list.emplace_back(taskResult.value());
  }
  return {};
}

void TaskList::removeTask(const size_t index) noexcept { list.erase(list.begin() + index); }

void TaskList::completeTask(const size_t index) noexcept {
  try {
    list.at(index).markComplete();
  } catch (const std::out_of_range &e) {
    return;
  }
}

void TaskList::removeCompletedTasks() noexcept {
  std::erase_if(list, [](const Task &task) { return task.isComplete(); });
}

void TaskList::removeAllTasks() { list.clear(); }

std::optional<std::vector<Task>> TaskList::getList() const noexcept {
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
  task_table.add_row({"#", "Task", "Status", "Due Date"});
  task_table[0]
      .format()
      .font_style({FontStyle::bold})
      .font_background_color(Color::white)
      .font_color(Color::grey);

  for (size_t i = 0; i < list.size(); ++i) {
    auto dateStr = list[i]
                       .getDueDate()
                       .transform([](const auto &date) { return Date::toString(date); })
                       .value_or("");
    auto completion = list[i].isComplete() ? "DONE" : "TODO";
    task_table.add_row({std::to_string(i + 1), list[i].getName(), completion, dateStr});
  }

  std::cout << task_table << std::endl;
}

std::optional<size_t> TaskList::findTask(const std::string &input) const {
  auto it = std::find_if(list.begin(), list.end(), [&input](const Task &task) {
    return task.getName().compare(0, input.length(), input) == 0;
  });
  if (it == list.end()) {
    return std::nullopt;
  }
  return it - list.begin();
}

std::optional<Task> TaskList::getTask(const size_t index) const {
  try {
    return list.at(index);
  } catch (const std::out_of_range &e) {
    return std::nullopt;
  }
}
