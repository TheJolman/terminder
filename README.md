# Task - Simple Task Manager CLI

## Description

Task is a simple, command-line task manager designed to help you keep track of your daily tasks. It's straightforward and easy to use, providing basic functionality for adding, completing, and deleting tasks, as well as viewing your current task list.

## Dependencies

- clang
- GNU Make

## Installation

1. Clone the repo:  
`git clone https://github.com/TheJolman/task`

2. Navigate to the project directory:  
`cd task`

3. Compile the project:  
`make`

4. Install the binary:
   
*this installs to /usr/local/bin by default and creates a file at /etc/task/savedata.txt and will ask for sudo priveledges. You can change the location of the savedata file in main.cpp and manually move the binary to somewhere else in your path if you do not wish to do this*

`make install`

## Usage

Task supports the following commands:

- **Add a Task**: `task add <mm/dd/yy>` - Adds a new task with an optional specified due date.
- **Complete a Task**: `task complete <task name>` - Marks a specified task as complete.
- **Delete a Task**: 
- `task del <task name>` - Removes a specified task from the list.
- `task del` - Removes all completed tasks if run with no arguments.
- **View Tasks**: `task view` - Displays all tasks.
- **Help**: `task help` - Displays available commands.

## Contributing

Contributions to Task are welcome! I'm not keeping the issues section updated but PRs are welcome.

## License

This project is licensed under the GNU General Public License v3.0. See the LICENSE file for more details.

