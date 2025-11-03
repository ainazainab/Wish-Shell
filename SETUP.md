# Project Setup Guide

## Project Overview
This is a Unix shell implementation project. You need to build a simple shell called `wish` (Wisconsin Shell) that can:
- Execute commands
- Support built-in commands (exit, cd, path)
- Handle redirection (>, redirects both stdout and stderr)
- Support parallel commands (&)

## Development Environment

Since this is a Unix/Linux project, you'll need a Unix-like environment. Options:

1. **WSL (Windows Subsystem for Linux)** - Recommended for Windows
   - Install WSL: `wsl --install` in PowerShell (as administrator)
   - Use Ubuntu distribution
   - Access from: `wsl` command in PowerShell

2. **School VM (VDI Platform)**
   - As mentioned in the project instructions
   - Access via browser or remote desktop client

3. **Local Virtual Machine**
   - VirtualBox or VMware with Ubuntu

## Building the Project

Once you have a Unix-like environment:

```bash
cd processes-shell
make
```

This will create the `wish` executable.

## Testing

Run the test suite:

```bash
./test-wish.sh
```

Or run specific tests:

```bash
./tester/run-tests.sh -t 1
```

## Implementation Steps

1. **Start with basic command execution**
   - Parse simple commands (no arguments)
   - Fork and exec to run programs
   - Wait for completion

2. **Add built-in commands**
   - Implement `exit`
   - Implement `cd` (change directory)
   - Implement `path` (set search path)

3. **Add argument parsing**
   - Handle commands with arguments
   - Properly tokenize input

4. **Add redirection**
   - Detect `>` operator
   - Redirect stdout and stderr to file

5. **Add parallel commands**
   - Detect `&` operator
   - Launch multiple processes in parallel
   - Wait for all to complete

## Important Notes

- Use `execv` (not `execvp` or `system()`)
- All errors should print: "An error has occurred\n" to stderr
- Initial path is `/bin`
- Use `getline()` for input
- Use `strsep()` for parsing
- Check return codes of all system calls

## Resources

- `fork()`, `exec()`, `wait()` man pages
- Project README.md for detailed specifications
- Test files in `tests/` directory show expected behavior

Good luck with your implementation!

