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
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <print>

struct Command {
  std::string name;
  std::string description;
  int minArgs;
  int maxArgs;
};

std::string findClosestTask(const TaskList &taskList,
                            const std::string &input) {
  auto tasks = taskList.getList();
  if (!tasks.has_value())
    return "";

  auto it = std::find_if(
      tasks.value().begin(), tasks.value().end(), [&input](const Task &task) {
        return task.getName().compare(0, input.length(), input) == 0;
      });
  return (it != tasks.value().end()) ? it->getName() : "";
}

int main(int argc, char *argv[]) {
  TaskList taskList;

  try {
    taskList.loadFromFile();
  } catch (const std::exception &e) {
    std::cerr << "Error loading tasks: " << e.what() << "\n";
  }

  CLI::App app{"Terminder: a task tracking CLI"};
  // Running with no subcommand will do the equivalent of `argv[0] ls`
  app.require_subcommand(0, 1);

  CLI::App *add =
      app.add_subcommand("add", "Create a new task with optional due date");

  std::string task_name{};
  add->add_option("task name", task_name, "Task to add");

  add->callback([&]() {
    if (!task_name.empty()) {
      taskList.addTask(task_name);
      std::println("Task {} added.", task_name);
    }
  });

  CLI::App *ls = app.add_subcommand("ls", "List all tasks");
  CLI::App *done = app.add_subcommand("done", "Mark a task as complete");
  CLI::App *rm = app.add_subcommand("rm", "Delete a task");
  CLI::App *clear = app.add_subcommand("clear", "Remove completed tasks");

  // try {
  //   if (*add) {
  //     if (argc < 3 || argc > 4) {
  //       std::cerr << "Error: Incorrect number of arguments for add command.\n";
  //       return 1;
  //     }
  //     std::string name = argv[2];
  //     if (argc == 4) {
  //       std::string dueDate = argv[3];
  //       taskList.addTask(name, dueDate);
  //       std::cout << "Task '" << name << "' added with due date " << dueDate
  //                 << ".\n";
  //     } else {
  //       taskList.addTask(name);
  //       std::cout << "Task '" << name << "' added.\n";
  //     }
  //   } else if (*ls) {
  //     auto tasks = taskList.getList();
  //     if (tasks.has_value()) {
  //       for (const auto &task : tasks.value()) {
  //         std::cout << task << "\n";
  //       }
  //     } else {
  //       std::cout << "No tasks found.\n";
  //     }
  //   } else if (*done) {
  //     if (argc != 3) {
  //       std::cerr << "Error: Task name required for complete command.\n";
  //       return 1;
  //     }
  //     std::string inputName = argv[2];
  //     std::string name = findClosestTask(taskList, inputName);
  //     if (name.empty()) {
  //       std::cerr << "Error: No task found matching '" << inputName << "'.\n";
  //       return 1;
  //     }
  //     taskList.completeTask(name);
  //     std::cout << "Task '" << name << "' marked as complete.\n";
  //   } else if (*rm) {
  //     if (argc != 3) {
  //       std::cerr << "Error: Task name required for delete command.\n";
  //       return 1;
  //     }
  //     std::string inputName = argv[2];
  //     std::string name = findClosestTask(taskList, inputName);
  //     if (name.empty()) {
  //       std::cerr << "Error: No task found matching '" << inputName << "'.\n";
  //       return 1;
  //     }
  //     taskList.removeTask(name);
  //     std::cout << "Task '" << name << "' deleted.\n";
  //   } else if (*clear) {
  //     try {
  //       taskList.removeAllTasks();
  //     } catch (const std::exception &e) {
  //       std::cerr << "Error: " << e.what() << "\n";
  //     }
  //     std::cout << "All tasks cleared.\n";
  //   }
  // } catch (const std::exception &e) {
  //   std::cerr << "Error: " << e.what() << "\n";
  //   return 100;
  // }

  CLI11_PARSE(app, argc, argv);

  // save tasks at end
  try {
    taskList.saveToFile();
    /*std::cout << "Tasks saved.\n";*/
  } catch (const std::exception &e) {
    std::cerr << "Error saving tasks: " << e.what() << "\n";
    return 100;
  }

  return 0;
}
