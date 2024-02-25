# Task - Simple Task Manager CLI

## Description

Task is a simple, command-line task manager designed to help you keep track of your daily tasks. It's straightforward and easy to use, providing basic functionality for adding, completing, and deleting tasks, as well as viewing your current task list.

## Dependencies

- C++17 compatible compiler (developed with `clang`)
- GNU Make

## Installation

1. Clone the repo:  
`git clone [repo url]`

2. Navigate to the project directory:  
`cd task`

3. Compile the project:  
`make`

This will create the `task` executable in the `build` directory.

## Usage

Task supports the following commands:

- **Add a Task**: `add <mm/dd/yy>` - Adds a new task with a specified due date.
- **Complete a Task**: `complete <task name>` - Marks a specified task as complete.
- **Delete a Task**: 
- `del <task name>` - Removes a specified task from the list.
- `del` - Removes all completed tasks if run with no arguments.
- **View Tasks**: `view` - Displays all tasks.
- **Help**: `help` - Displays available commands.

To run the task manager, use the following command from the project's root directory:  
`./build/task`

You might want to move the binary to somewhere in your PATH, however.

## Contributing

Contributions to Task are welcome! Refer to the the project's issues section for any guidance on contributing.

## License

This project is licensed under the GNU General Public License v3.0. See the LICENSE file for more details.

