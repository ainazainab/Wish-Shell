Wish Shell – Custom Unix Command-Line Shell

Wish Shell is a lightweight Unix-style command-line interpreter built in C. It supports both interactive and batch execution modes, executing system commands just like a basic shell. This project focuses on process management, command parsing, output redirection, parallel execution, and handling built-in shell commands using Linux system calls.

🚀 Features

Interactive mode (wish>) and batch mode (run commands from a file)

Executes external programs using fork(), execv(), and wait()

Built-in commands:

exit — terminates the shell

cd — changes the current working directory

path — manages directories used to search for executables

Supports output redirection (>) to redirect both stdout and stderr

Parallel command execution using &

Robust single-style error handling

Clean command parsing using getline() and strsep()

⚙️ How It Works

The shell runs in an infinite loop:

Displays prompt (wish>) in interactive mode

Reads user input using getline()

Parses the command using strsep()

Checks whether it's a built-in command or an external command

Executes using fork, execv, and wait

Repeats until exit is called

📌 Usage
Interactive Mode
./wish
wish> ls -l
wish> cd /tmp
wish> exit

Batch Mode
./wish commands.txt


In batch mode, commands are executed from the file without showing the prompt.

📁 Output Redirection

Redirects both stdout and stderr to a file using >:

wish> ls -la /tmp > output.txt


📝 Notes:

Overwrites file if it already exists

Supports only one redirection operator

Multiple redirections are considered an error

⚡ Parallel Command Execution

Run multiple commands at the same time using &:

wish> cmd1 & cmd2 arg1 arg2 & cmd3


Each command runs in a separate process before the shell waits for all to complete.

🔌 Built-in Commands
Command	Description
exit	Exits the shell
cd <directory>	Changes working directory
path <dir1> <dir2> ...	Sets the executable search path

Example:

wish> path /bin /usr/bin

❗ Error Handling

The shell uses one consistent error message for any type of failure:

An error has occurred


This applies to syntax errors, missing files, invalid commands, and more.

📂 Project Structure
wish-shell/
│── wish.c
│── Makefile
│── README.md
│── tests/
│   ├── test1.in
│   ├── test1.out
│   ├── ...
│── test-wish.sh

🛠 Compilation and Run
make
./wish

🧪 Testing

Use the included script to automatically test your shell:

bash test-wish.sh

🧠 Key Concepts Learned

Process creation and management (fork, execv, wait)

Command parsing and input handling

File I/O redirection using system calls

Parallel execution using child processes

Building a shell-like CLI environment in C

📜 License

This project is open-source and available for learning and personal use.
