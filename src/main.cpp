/**
 * @file main.cpp
 * @brief Main function for task program
 * @author Joshua Holman
 * @contact jolman@duck.com
 *
 * @copyright Copyright (c) 2024 Joshua Holman
 *
 * This source code is licensed under the MIT License.
 * See the LICENSE file in the project root for full license information.
 */

#include "CLI/CLI.hpp"
#include "TaskList.hpp"
#include "Util.hpp"
#include <expected>
#include <print>
#include <string>

bool isPosNum(const std::string &s) {
  return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

size_t getValidIndexOrThrow(TaskList &taskList, const std::string &input) {
  if (input.empty()) {
    util::error("Name or index required");
    throw CLI::RuntimeError(1);
  }
  size_t index{};
  if (isPosNum(input)) {
    // check if index exists if arg is numeric
    index = std::stoi(input);
    auto result = taskList.getTask(index);
    if (!result) {
      util::error("Task not found");
      throw CLI::RuntimeError(1);
    }
  } else {
    // If arg is non-numeric try to find string in tasktaskList
    auto result = taskList.findTask(input);
    if (!result) {
      util::error("Task not found");
      throw CLI::RuntimeError(1);
    }
    index = result.value();
  }
  return index;
}

int main(int argc, char *argv[]) {
  TaskList taskList;

  auto loadResult = taskList.loadFromFile();
  if (!loadResult) {
    util::error("couldn't load tasks: {}", loadResult.error());
  }

  CLI::App app{"Terminder: a task tracking CLI"};
  app.set_version_flag("--version,-v", VERSION);
  app.require_subcommand(1);

  // Add Subcommand ===============================================================================
  CLI::App *add = app.add_subcommand("add", "Create a new task with optional due date");

  std::string nameInput{};
  add->add_option("name", nameInput, "Task to add");

  std::string dateInput{};
  add->add_option("--date,-d", dateInput, "Optional date (format: mm/dd/yy)");

  add->callback([&]() {
    if (nameInput.empty()) {
      util::error("a name is required");
      throw CLI::RuntimeError(1);
    }
    if (taskList.findTask(nameInput)) {
      util::error("task '{}' already exists", nameInput);
      throw CLI::RuntimeError(1);
    }

    if (!dateInput.empty()) {
      auto result = Date::fromString(dateInput);
      if (!result) {
        util::error("{}", result.error());
        throw CLI::RuntimeError(1);
      }
      taskList.addTask(Task(nameInput, result.value()));
      std::println("Task '{}' added with due date {}.", nameInput, dateInput);
    } else {
      taskList.addTask(Task(nameInput));
      std::println("Task '{}' added.", nameInput);
    }
  });

  // List Subcommand ==============================================================================
  CLI::App *ls = app.add_subcommand("ls", "List all tasks");
  ls->callback([&]() { taskList.prettyPrint(); });

  // Done Subcommand ==============================================================================
  CLI::App *done = app.add_subcommand("done", "Mark a task as complete");
  std::string input{};
  done->add_option("name", input, "index or name (partial OK) of task to complete");

  done->callback([&]() {
    size_t index = getValidIndexOrThrow(taskList, input);
    taskList.completeTask(index);
    std::println("Task marked as complete.");
  });

  // Remove Subcommand ============================================================================
  CLI::App *rm = app.add_subcommand("rm", "Delete a task");
  rm->add_option("name", input, "Name or pattern to delete task(s)");
  rm->callback([&]() {
    size_t index = getValidIndexOrThrow(taskList, input);
    taskList.removeTask(index);
    std::println("Task deleted.");
  });

  // Clear Subcommand =============================================================================
  CLI::App *clear = app.add_subcommand("clear", "Remove completed tasks");
  bool all_flag{false};
  clear->add_flag("--all,-a", all_flag, "Remove all tasks instead");
  clear->callback([&]() {
    if (all_flag) {
      taskList.removeAllTasks();
      std::println("All tasks cleared.");
    } else {
      taskList.removeCompletedTasks();
      std::println("Completed tasks cleared.");
    }
  });

  CLI11_PARSE(app, argc, argv);

  // save tasks at end
  auto saveResult = taskList.saveToFile();
  if (!saveResult) {
    util::error("could not save tasks: {}", saveResult.error());
    return 100;
  }

  return 0;
}
