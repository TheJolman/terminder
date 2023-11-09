// Task.h
#pragma once
#include <string>
#include "Date.h"
#include <sstream>

class Task {
private:

  std::string name;
  Date dueDate; 
  bool completion;

public:
  Task(std::string name, Date dueDate) : name(name), dueDate(dueDate), completion(false) {}
  Task(std::string name, Date dueDate, bool completion) : name(name), dueDate(dueDate), completion(completion) {}

  void markComplete() { completion = true; }
  
  std::string getName() const { return name; }

  friend std::ostream& operator<<(std::ostream& os, const Task& t) {
    std::string status = (t.completion ? "Completed" : "Incomplete");
    os << t.name << "\t" << t.dueDate << "\t" << status;
    return os;
  }

  std::string serialize() const {
    std::ostringstream oss;
    oss << name << ',' << dueDate << ',' << completion;
    return oss.str();
  }

  static Task deserialize(const std::string &data) {
    std::istringstream iss(data);
    std::string token;
    std::getline(iss, token, ',');
    std::string name = token;
    std::getline(iss, token, ',');
    Date date;
    date.parseDate(token);
    std::getline(iss, token, ',');
    bool completion;
    if (token == "1")
      completion = true;
    else
      completion = false;
    return Task(name, date, completion);
  }
};
