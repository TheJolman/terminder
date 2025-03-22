# Terminder - Simple Task Manager CLI

## Description

Terminder is a dead simple task manager cli that I wrote for my own purposes. Uses the ceral serialization library.

## Dependencies

- cereal
- clang++
- GNU Make

## Installation

For nix users: add the following to the inputs of your `flake.nix`
```nix
terminder.url = "github:thejolman/terminder"
```
And then you can reference it with
```nix
terminder.packages.${system}.default
```

For everyone else: clone and then `make && make install` (installs to ~/.local/usr/bin by default)  

## Usage

Terminder supports the following commands:

- **Add a task**: `task add <mm/dd>` - Adds a new task with an optional specified due date.
- **Complete a task**: `task complete <task name>` - Marks a specified task as complete.
- **Delete a task**: `task delete <task name>` - Removes a specified task from the list.
- **View tasks**: `task list` - Displays all tasks.
- **Help**: `task help` - Displays available commands.

## Contributing

Contributions to Terminder are welcome! I'm not keeping the issues section updated but PRs are welcome.

## License  

MIT License. Do whatever you want with this.  
