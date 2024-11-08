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

#include "Date.hpp"
#include "Task.hpp"
#include "TaskList.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Command {
  std::string name;
  std::string description;
  int minArgs;
  int maxArgs;
};

std::vector<Command> commands{
    {"add", "Add a new task (usage: add <name> [due_date])", 1, 2},
    {"list", "List all tasks", 0, 0},
    {"complete", "Makr a task as complete (uasge: complete <nome>)", 1, 1},
    {"delete", "Delete a specific task (usage: delete <nome>)", 1, 1},
    {"clear", "Delete all tasks", 0, 0},
    {"help", "Show this help message", 0, 0}};

void printHelp() {
  std::cout << "Usage task [command] [arguments]\n"
            << "Commands:\n";
  for (const auto &cmd : commands) {
    std::cout << "  " << cmd.name << " - " << cmd.description << "\n";
  }
}

std::string findClosestCommand(const std::string &input) {
  auto it = std::find_if(
      commands.begin(), commands.end(), [&input](const Command &cmd) {
        return cmd.name.compare(0, input.length(), input) == 0;
      });
  return (it != commands.end()) ? it->name : "";
}

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

// TODO: add option for clearing completed tasks
int main(int argc, char *argv[]) {
  TaskList taskList;

  try {
    taskList.loadFromFile();
    /*std::cout << "Tasks loaded.\n";*/
  } catch (const std::exception &e) {
    std::cerr << "Error loading tasks: " << e.what() << "\n";
  }

  if (argc < 2) {
    std::cerr << "Error: No command provided.\n";
    printHelp();
    return 1;
  }

  std::string inputCommand = argv[1];
  std::string command = findClosestCommand(inputCommand);

  if (command.empty()) {
    std::cerr << "Error: Unknown command '" << inputCommand << "'.\n";
    printHelp();
    return 1;
  }

  try {
    if (command == "add") {
      if (argc < 3 || argc > 4) {
        std::cerr << "Error: Incorrect number of arguments for add command.\n";
        return 1;
      }
      std::string name = argv[2];
      if (argc == 4) {
        std::string dueDate = argv[3];
        taskList.addTask(name, dueDate);
        std::cout << "Task '" << name << "' added with due date " << dueDate
                  << ".\n";
      } else {
        taskList.addTask(name);
        std::cout << "Task '" << name << "' added.\n";
      }
    } else if (command == "list") {
      auto tasks = taskList.getList();
      if (tasks.has_value()) {
        for (const auto &task : tasks.value()) {
          std::cout << task << "\n";
        }
      } else {
        std::cout << "No tasks found.\n";
      }
    } else if (command == "complete") {
      if (argc != 3) {
        std::cerr << "Error: Task name required for complete command.\n";
        return 1;
      }
      std::string inputName = argv[2];
      std::string name = findClosestTask(taskList, inputName);
      if (name.empty()) {
        std::cerr << "Error: No task found matching '" << inputName << "'.\n";
        return 1;
      }
      taskList.completeTask(name);
      std::cout << "Task '" << name << "' marked as complete.\n";
    } else if (command == "delete") {
      if (argc != 3) {
        std::cerr << "Error: Task name required for delete command.\n";
        return 1;
      }
      std::string inputName = argv[2];
      std::string name = findClosestTask(taskList, inputName);
      if (name.empty()) {
        std::cerr << "Error: No task found matching '" << inputName << "'.\n";
        return 1;
      }
      taskList.removeTask(name);
      std::cout << "Task '" << name << "' deleted.\n";
    } else if (command == "clear") {
      try {
        taskList.removeAllTasks();
      } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n";
      }
      std::cout << "All tasks cleared.\n";
    } else if (command == "help" || command == "-h" || command == "--help") {
      printHelp();
    } else {
      std::cerr << "Error: Unknown command '" << command << "'.\n";
      printHelp();
      return 1;
    }
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 100;
  }

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
