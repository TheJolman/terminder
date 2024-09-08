/*
 * Project: task
 * File: main.cpp
 * Description: main function
 * Copyright (C) 2024 Joshua Holman
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Joshua Holman
 * Contact: jolman@duck.com
 */

#include <iostream>
#include <list>
#include <fstream>
#include "Date.hpp"
#include "Task.hpp"
#include <map>

// Change this if you want to save the file to a different location
const std::string saveFilePath = "./savedata.txt";

void help();
void add(int, char *[], std::list<Task> &);
void complete(int, char *[], std::list<Task> &);
void del(int, char *[], std::list<Task> &);
void clear(std::list<Task> &);
void list(std::list<Task> &);

void saveTasksToFile(const std::list<Task> &, const std::string &);
void loadTasksFromFile(std::list<Task> &, const std::string &);

enum Command {
  ADD,
  COMPLETE,
  DELETE,
  CLEAR,
  HELP,
  LIST,
  UNKNOWN
};

std::map<std::string, Command> commandMap = {
  {"add", ADD},
  {"complete", COMPLETE},
  {"del", DELETE},
  {"clear", CLEAR},
  {"help", HELP},
  {"list", LIST}
};
  

int main(int argc, char *argv[]) {
  std::list<Task> tasklist;

  if (argc == 1) {
    std::cerr << "Please provide an argument. See " << argv[0] << " help for a list of commands." << std::endl;
    return 1;
  }

  if (argc > 4) {
    std::cerr << "Too many arguments. See " << argv[0] << " help for a list of commands." << std::endl;
    return 1;
  }

  std::string modifier = argv[1];
  Command command = commandMap.count(modifier) ? commandMap[modifier] : UNKNOWN;

  switch (command) {
    case ADD:
      add(argc, argv, tasklist);
        break;
    case COMPLETE:
      complete(argc, argv, tasklist);
      break;
    case CLEAR:
      clear(tasklist);
      break;
    case DELETE:
      del(argc, argv, tasklist);
      break;
    case HELP:
      help();
      break;
    case LIST:
      list(tasklist);
      break;
    default:
      std::cerr << "Unknown command: " << modifier << std::endl;
      std::cerr << "Try " << argv[0] << " help for a list of commands." << std::endl;
      return 1;
  }

  for (auto &obj : tasklist) {
      std::cout << obj << "\n";
  }

  return 0;
}

void saveTasksToFile(const std::list<Task> &tasklist, const std::string &filename) {
  std::ofstream file(filename);
  for (const auto& task : tasklist) {
    file << task.serialize() << std::endl;
  }
  file.close();
}

void loadTasksFromFile(std::list<Task>& tasklist, const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        try {
            tasklist.push_back(Task::deserialize(line));
        } catch (const std::invalid_argument& e) {
            std::cerr << "Warning: Skipping invalid task data: " << line << std::endl;
        }
    }
    file.close();
}

void help() {
    std::cout << "task [option] [arguments]\n"
              << "Options:\n"
              << "list                    - List all tasks\n"
              << "add [name] [due date]   - Adds a task with optional due date\n"
              << "complete [name]         - Marks a task as complete\n"
              << "del [name]              - Deletes a task, deleted all completed tasks if given no argument\n"
              << "clear                   - Deletes all tasks\n"
              << "help                    - Displays this message\n";  
}

void add(int argc, char *argv[], std::list<Task> &tasklist) {
    if (argc < 3) {
        std::cerr << "Not enough arguments for 'add' command." << std::endl;
        return;
    }

    std::string taskname = argv[2];
    std::string dateStr = (argc > 3) ? argv[3] : "";
    Date dueDate;

    if (!dateStr.empty()) {
        try {
            dueDate = Date(dateStr);
        } catch (const std::invalid_argument &e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return;
        }
    }

    loadTasksFromFile(tasklist, saveFilePath);
    tasklist.emplace_back(taskname, dueDate);
    saveTasksToFile(tasklist, saveFilePath);
}

void complete(int argc, char *argv[], std::list<Task> &tasklist) {
  if (argc < 3) {
    std::cerr << "Not enough arguments for 'complete' command." << std::endl;
    return;
  }

  std::string taskname = argv[2];
  loadTasksFromFile(tasklist, saveFilePath);
  for (auto &task : tasklist) {
    if (task.getName() == taskname) {
      task.markComplete();
      break;
    }
  }
  saveTasksToFile(tasklist, saveFilePath);
}

void del(int argc, char *argv[], std::list<Task> &tasklist) {
  loadTasksFromFile(tasklist, saveFilePath);
  if (argc < 3) {
    tasklist.remove_if([](const Task &task) { return task.getCompletion(); });
  } else {
    std::string taskname = argv[2];
    tasklist.remove_if([&](const Task &task) { return task.getName() == taskname; });
  }
    saveTasksToFile(tasklist, saveFilePath);
}

void clear(std::list<Task> &tasklist) {
  tasklist.clear();
  saveTasksToFile(tasklist, saveFilePath);
}

void list(std::list<Task> &tasklist) {
  loadTasksFromFile(tasklist, saveFilePath);
}
