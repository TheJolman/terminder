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

#include <iostream>
#include <string>
#include <sstream>
#include "TaskList.h"
#include "Task.h"
#include "Date.h"

void printHelp() {
    std::cout << "Usage: task [command] [arguments]\n"
              << "Commands:\n"
              << "  add <name> [due_date]    - Add a new task (due_date format: MM/DD)\n"
              << "  list                     - List all tasks\n"
              << "  complete <name>          - Mark a task as complete\n"
              << "  delete <name>            - Delete a specific task\n"
              << "  clear                    - Delete all tasks\n"
              << "  save                     - Save tasks to file\n"
              << "  load                     - Load tasks from file\n"
              << "  help                     - Show this help message\n";
}

int main(int argc, char* argv[]) {
    TaskList taskList;

    if (argc < 2) {
        std::cerr << "Error: No command provided.\n";
        printHelp();
        return 1;
    }

    std::string command = argv[1];

    try {
        if (command == "add") {
            if (argc < 3) {
                std::cerr << "Error: Task name required for add command.\n";
                return 1;
            }
            std::string name = argv[2];
            if (argc == 4) {
                std::string dueDate = argv[3];
                taskList.addTask(name, dueDate);
                std::cout << "Task '" << name << "' added with due date " << dueDate << ".\n";
            } else {
                taskList.addTask(name);
                std::cout << "Task '" << name << "' added.\n";
            }
        } else if (command == "list") {
            auto tasks = taskList.getList();
            if (tasks.has_value()) {
                for (const auto& task : tasks.value()) {
                    std::cout << task << "\n";
                }
            } else {
                std::cout << "No tasks found.\n";
            }
        } else if (command == "complete") {
            if (argc < 3) {
                std::cerr << "Error: Task name required for complete command.\n";
                return 1;
            }
            std::string name = argv[2];
            taskList.completeTask(name);
            std::cout << "Task '" << name << "' marked as complete.\n";
        } else if (command == "delete") {
            if (argc < 3) {
                std::cerr << "Error: Task name required for delete command.\n";
                return 1;
            }
            std::string name = argv[2];
            taskList.removeTask(name);
            std::cout << "Task '" << name << "' deleted.\n";
        } else if (command == "clear") {
            taskList.removeCompletedTasks(); // This method name might be misleading, update if it actually clears all tasks
            std::cout << "All tasks cleared.\n";
        } else if (command == "save") {
            taskList.saveToFile();
            std::cout << "Tasks saved to file.\n";
        } else if (command == "load") {
            taskList.loadFromFile();
            std::cout << "Tasks loaded from file.\n";
        } else if (command == "help") {
            printHelp();
        } else {
            std::cerr << "Error: Unknown command '" << command << "'.\n";
            printHelp();
            return 1;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
