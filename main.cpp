// main.cpp
#include <iostream>
#include <list>
#include "Date.h"
#include "Task.h"

int main(int argc, char *argv[]) {
  std::list<Task> tasklist;

  if (argc != 4) {
    std::cerr << "Usage: " << argv[0] << " add taskname date" << std::endl;
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

    std::cout << "Task: " << taskname << "\n";
    std::cout << "Due: " << date << std::endl;
  }
  else {
    std::cerr << "Unknown command: " << modifier << std::endl;
    return 1;
  }

  return 0;
}
