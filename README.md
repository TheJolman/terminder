# Terminder - Simple Task Manager CLI

## Description

Terminder is a dead simple task manager cli that I wrote for my own purposes. Uses the cereal serialization library.

## Dependencies

- cereal
- cli11
- recent version of clang++ (this project uses c++23)
- GNU Make

## Installation

For Nix users: add the following to the inputs of your `flake.nix`
```nix
terminder.url = "github:thejolman/terminder"
```
And then you can reference it with the following anywhere in your config
```nix
inputs.terminder.packages.${system}.default
```

For everyone else: install with the Makefile
```sh
make && sudo make install
```
You can adjust the install location like:
```sh
# This will install to ~/.local/bin/
make install PREFIX=~/.local
```

## Usage

```
Terminder: a task tracking CLI


terminder [OPTIONS] SUBCOMMAND


OPTIONS:
  -h,     --help              Print this help message and exit

SUBCOMMANDS:
  add                         Create a new task with optional due date
  ls                          List all tasks
  done                        Mark a task as complete
  rm                          Delete a task
  clear                       Remove completed tasks
```

## Contributing

Contributions to Terminder are welcome. I'm a student so any submitted issues/prs are appreciated.
