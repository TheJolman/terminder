// main.cpp
#include <iostream>
#include <list>
#include <fstream>
#include <algorithm>
#include "Date.h"
#include "Task.h"

// TODO: add some argc error checking with the specific commands

void saveTasksToFile(const std::list<Task> &, const std::string &);
void loadTasksFromFile(std::list<Task> &, const std::string &);

int main(int argc, char *argv[]) {
  std::list<Task> tasklist;

  if (argc > 4) {
    std::cerr << "Too many arguments. See " << argv[0] << " help for a list of commands." << std::endl;
    return 1;
  }

  std::string modifier = argv[1];
  std::string taskname;
  Date date;

  if (modifier == "add") {
    taskname = argv[2];
    std::string dateStr = argv[3];
    Date date;
    try {
      Date tempDate(dateStr);
      date = tempDate;
    } 
    catch (const std::invalid_argument &e) {
      std::cerr << "FUCK YOU: " << e.what() << std::endl;
    }
    
    loadTasksFromFile(tasklist, "saveData.txt");
    tasklist.emplace_back(taskname, date);
    saveTasksToFile(tasklist, "saveData.txt");

  }
  else if (modifier == "complete") {
    loadTasksFromFile(tasklist, "saveData.txt");
    std::string nameToFind = argv[2];

    auto it = std::find_if(tasklist.begin(), tasklist.end(), [nameToFind](const Task &obj) {
      return obj.getName() == nameToFind;
    });

    it->markComplete();
    saveTasksToFile(tasklist, "saveData.txt");
  }
  else if (modifier == "clear") {
    loadTasksFromFile(tasklist, "saveData.txt");
    tasklist.clear();
    saveTasksToFile(tasklist, "saveData.txt");
  }
  else if (modifier == "delete" || modifier == "del") { // lst.erase(iterator)
    loadTasksFromFile(tasklist, "saveData.txt");
    std::string nameToFind = argv[2];

    auto it = std::find_if(tasklist.begin(), tasklist.end(), [nameToFind](const Task &obj) {
      return obj.getName() == nameToFind;
    });

    if (it != tasklist.end()) {
      tasklist.erase(it);
      std::cout << "Task '" << nameToFind << "' deleted successfully.\n";
    }
    else {
      std::cerr << "Task '" << nameToFind << "' not found.\n";
    }
    saveTasksToFile(tasklist, "saveData.txt");
  }
  else if (modifier == "help") {
    std::cout << argv[0] << " view\n";
    std::cout << argv[0] << " add [name] [due date]\n";
    std::cout << argv[0] << " complete [name]\n";
    std::cout << argv[0] << " del [name]\n";
    std::cout << "Note: " << argv[0] << " del with no arguments will delete all completed tasks.\n";
    std::cout << argv[0] << " clear\n";
    std::cout << argv[0] << " help\n";
  }
  else if (modifier == "view") {
    loadTasksFromFile(tasklist, "saveData.txt");
  }
  else {
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

void loadTasksFromFile(std::list<Task> &tasklist, const std::string &filename) {
  std::ifstream file(filename);
  std::string line;
  while (std::getline(file, line)) {
    tasklist.push_back(Task::deserialize(line));
  }
  file.close();
}
