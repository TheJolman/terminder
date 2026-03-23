# Terminder - Simple Task Manager CLI

```
+---+--------------+--------+-------------+
| # | Task         | Status | Due Date    |
+---+--------------+--------+-------------+
| 1 | squash bugs  | DONE   |             |
+---+--------------+--------+-------------+
| 2 | make release | TODO   | Apr 01 2026 |
+---+--------------+--------+-------------+
```

## Description

Terminder is a dead simple task manager cli that I wrote for my own purposes. Uses the cereal serialization library.

## Dependencies

- cereal
- cli11

## Installation

### Nix

```sh
nix run github:thejolman/terminder
```

You can also add this to your flake inputs and reference it in your NixOS or Home Manager config to install globally.

### Everyone else

```sh
make && sudo make install
```
You can adjust the install location like:
```sh
# This will install to ~/.local/bin/
make install prefix=~/.local
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

Tasks are stored at `$XDG_DATA_HOME/terminder/data.json`. This is usually `~/.local/share/terminder/data.json`.

## Contributing

Contributions to Terminder are welcome. I'm a student so any submitted issues/prs are appreciated.

## Related Projects
- [Taskwarrior](https://taskwarrior.org/docs/start/)
