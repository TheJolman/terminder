// Task.h
#pragma once
#include <string>
#include "Date.h"

class Task {
private:

  std::string name;
  bool completion;
  Date dueDate; 

public:
  Task(std::string name, Date dueDate) : name(name), dueDate(dueDate), completion(false) {}

  friend std::ostream& operator<<(std::ostream& os, const Task& t) {
    std::string status = (t.completion ? "Completed" : "Incomplete");
    os << t.name << "/" << t.dueDate << "/" << status;
    return os;
  }
};
