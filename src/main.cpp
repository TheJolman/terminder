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
    throw CLI::RuntimeError("Name or index required", 1);
  }
  size_t index{};
  if (isPosNum(input)) {
    // check if index exists if arg is numeric
    index = std::stoi(input);
    auto result = taskList.getTask(index);
    if (!result) {
      throw CLI::RuntimeError("Task not found", 1);
    }
  } else {
    // If arg is non-numeric try to find string in tasktaskList
    auto result = taskList.findTask(input);
    if (!result) {
      throw CLI::RuntimeError("Task not found", 1);
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

  std::string task_name{};
  add->add_option("name", task_name, "Task to add");

  std::string date_str{};
  add->add_option("--date,-d", date_str, "Optional date (format: mm/dd)");

  add->callback([&]() {
    if (task_name.empty()) {
      util::error("a name is required");
      return;
    }
    if (taskList.findTask(task_name)) {
      util::error("task '{}' already exists", task_name);
      exit(1);
    }

    auto addResult =
        date_str.empty() ? taskList.addTask(task_name) : taskList.addTask(task_name, date_str);

    if (!addResult) {
      util::error("failed to add task: {}", addResult.error());
      return;
    }

    if (date_str.empty()) {
      std::println("Task '{}' added.", task_name);
    } else {
      std::println("Task '{}' added with due date {}.", task_name, date_str);
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
