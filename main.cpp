// main.cpp
#include <iostream>
#include <list>
#include <fstream>
#include "Date.h"
#include "Task.h"

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
    if (!date.parseDate(dateStr)) {
      std::cerr << "Invalid date format or values." << std::endl;
      return 1;
    }
    
    loadTasksFromFile(tasklist, "saveData.txt");
    tasklist.emplace_back(taskname, date);
    saveTasksToFile(tasklist, "saveData.txt");

  }
  else if (modifier == "clear") {
    tasklist.clear();
  }
  else if (modifier == "delete" || modifier == "del") {

  }
  else if (modifier == "help") {
    std::cout << argv[0] << " add [name] [due date]\n";
    std::cout << argv[0] << " del [name]\n";
    std::cout << argv[0] << " complete [name]\n";
    std::cout << argv[0] << " clear\n";
    std::cout << argv[0] << " help\n";
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
