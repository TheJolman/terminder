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

// #include "Date.hpp"
#include "CLI/CLI.hpp"
#include "Task.hpp"
#include "TaskList.hpp"
#include <algorithm>
#include <cstdio>
#include <expected>
#include <print>
#include <sstream>
#include <string>
#include <vector>

namespace util {
template <typename... Args>
static void error(std::format_string<Args...> fmt, Args &&...args) {
  std::println(stderr, "[ERROR] {}",
               std::format(fmt, std::forward<Args>(args)...));
}

} // namespace util

struct Command {
  std::string name;
  std::string description;
  int minArgs;
  int maxArgs;
};

std::expected<Task, std::string> find_task(const TaskList &taskList,
                                           const std::string &input) {
  auto tasks = taskList.getList();
  if (!tasks.has_value()) {
    return std::unexpected("task list could not be retrieved");
  }

  auto it = std::find_if(
      tasks.value().begin(), tasks.value().end(), [&input](const Task &task) {
        return task.getName().compare(0, input.length(), input) == 0;
      });
  if (it == tasks.value().end()) {
    return std::unexpected("could not find task");
  }
  return *it;
}

int main(int argc, char *argv[]) {
  TaskList taskList;

  auto loadResult = taskList.loadFromFile();
  if (!loadResult) {
    util::error("couldn't load tasks: {}", loadResult.error());
  }

  CLI::App app{"Terminder: a task tracking CLI"};
  // maybe make running with no subcommand will do the equivalent of `argv[0]
  // ls`
  app.require_subcommand(1);

  CLI::App *add =
      app.add_subcommand("add", "Create a new task with optional due date");

  std::string task_name{};
  add->add_option("name", task_name, "Task to add");

  std::string date_str{};
  add->add_option("--date,-d", date_str, "Optional date");

  add->callback([&]() {
    if (task_name.empty()) {
      util::error("a name is required");
      return;
    }

    auto addResult = date_str.empty() ? taskList.addTask(task_name)
                                      : taskList.addTask(task_name, date_str);

    if (!addResult) {
      util::error("failed to add task: {}", addResult.error());
      return;
    }

    if (date_str.empty()) {
      std::println("Task {} added.", task_name);
    } else {
      std::println("Task {} added with due date {}.", task_name, date_str);
    }
  });

  CLI::App *ls = app.add_subcommand("ls", "List all tasks");
  ls->callback([&]() {
    auto tasks = taskList.getList();
    if (tasks.has_value()) {
      for (const auto &task : tasks.value()) {
        std::println("{}", task);
      }
      return;
    }
    std::println("No tasks found.");
  });

  CLI::App *done = app.add_subcommand("done", "Mark a task as complete");
  done->add_option("name", task_name, "Name or pattern to complete task(s)");

  done->callback([&]() {
    if (task_name.empty()) {
      util::error("a name is required");
      return;
    }

    auto task = find_task(taskList, task_name);
    if (!task) {
      util::error("no task found matching {}", task_name);
      return;
    }
    taskList.completeTask(task.value().getName());
    std::println("Task '{}' marked as complete.", task.value().getName());
  });

  CLI::App *rm = app.add_subcommand("rm", "Delete a task");
  rm->add_option("name", task_name, "Name or pattern to delete task(s)");
  rm->callback([&]() {
    if (task_name.empty()) {
      util::error("a name is required");
      return;
    }

    auto task = find_task(taskList, task_name);
    if (!task) {
      util::error("no task found matching {}", task_name);
      return;
    }
    taskList.removeTask(task.value().getName());
    std::println("Task '{}' marked as deleted.", task.value().getName());
  });

  CLI::App *clear = app.add_subcommand("clear", "Remove completed tasks");
  clear->callback([&]() {
    taskList.removeAllTasks();
    std::println("All tasks cleared.");
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
