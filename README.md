# Terminder - Simple Task Manager CLI

## Description

Terminder is a dead simple task manager cli that I wrote for my own purposes. Uses the cereal serialization library.

## Dependencies

- cereal
- clang++
- GNU Make

## Installation

For Nix users: add the following to the inputs of your `flake.nix`
```nix
terminder.url = "github:thejolman/terminder"
```
And then you can reference it with the following anywhere in your config
```nix
pkgs.inputs.terminder.packages.${system}.default
```

For everyone else: install with the Makefile
```sh
make && make install
```
You can adjust the install location like:
```sh
# This will install to ~/.local/bin/
make install PREFIX=~/.local
```

## Usage

Terminder supports the following commands:

- **Add a task**: `task add <mm/dd>` - Adds a new task with an optional due date.
- **Complete a task**: `task complete <task name>` - Marks a specified task as complete.
- **Delete a task**: `task delete <task name>` - Removes a specified task from the list.
- **View tasks**: `task list` - Displays all tasks.
- **Help**: `task help` - Displays available commands.

## Contributing

Contributions to Terminder are welcome. I'm a student so any submitted issues/prs are appreciated.
