/*
 * wish.c - A simple Unix shell implementation
 * 
 * This shell supports:
 * - Basic command execution
 * - Built-in commands: exit, cd, path
 * - Redirection with >
 * - Parallel commands with &
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>

#define MAX_LINE_LENGTH 512
#define MAX_ARGS 64
#define MAX_PATH_DIRS 64

/* Global variables */
char *path_dirs[MAX_PATH_DIRS];
int path_count = 0;

/* Function declarations */
void print_error(void);
void initialize_path(void);
void parse_path(char *path_str);
int is_builtin(char *cmd);
void execute_builtin(char **args, int arg_count);
void execute_command(char **args, int arg_count, char *output_file);
char *find_executable(char *cmd);
void parse_command(char *line, char **args, int *arg_count, 
                   char **output_file, int *is_parallel);

int main(int argc, char *argv[]) {
    FILE *input;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int interactive = 1;
    
    /* Initialize default path */
    initialize_path();
    
    /* Determine input source */
    if (argc > 2) {
        print_error();
        exit(1);
    }
    
    if (argc == 2) {
        /* Batch mode */
        input = fopen(argv[1], "r");
        if (input == NULL) {
            print_error();
            exit(1);
        }
        interactive = 0;
    } else {
        /* Interactive mode */
        input = stdin;
    }
    
    /* Main shell loop */
    while (1) {
        if (interactive) {
            printf("wish> ");
            fflush(stdout);
        }
        
        read = getline(&line, &len, input);
        
        if (read == -1) {
            /* EOF reached */
            if (line) {
                free(line);
            }
            if (input != stdin) {
                fclose(input);
            }
            exit(0);
        }
        
        /* Remove newline */
        if (line[read - 1] == '\n') {
            line[read - 1] = '\0';
            read--;
        }
        
        /* Skip empty lines */
        if (read == 0 || strspn(line, " \t") == read) {
            continue;
        }
        
        /* TODO: Parse and execute commands */
        /* This is where you'll need to implement:
         * 1. Parse the command line (handling &, >, etc.)
         * 2. Execute built-in commands or external commands
         * 3. Handle redirection
         * 4. Handle parallel commands
         */
    }
    
    if (line) {
        free(line);
    }
    if (input != stdin) {
        fclose(input);
    }
    
    return 0;
}

void print_error(void) {
    char error_message[30] = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message));
}

void initialize_path(void) {
    path_count = 1;
    path_dirs[0] = strdup("/bin");
    path_dirs[1] = NULL;
}

int is_builtin(char *cmd) {
    if (cmd == NULL) return 0;
    return (strcmp(cmd, "exit") == 0 || 
            strcmp(cmd, "cd") == 0 || 
            strcmp(cmd, "path") == 0);
}

/* TODO: Implement execute_builtin function */
void execute_builtin(char **args, int arg_count) {
    if (args[0] == NULL) return;
    
    if (strcmp(args[0], "exit") == 0) {
        if (arg_count > 1) {
            print_error();
            return;
        }
        exit(0);
    }
    
    /* TODO: Implement cd and path built-in commands */
}

/* TODO: Implement execute_command function */
void execute_command(char **args, int arg_count, char *output_file) {
    /* TODO: Find executable, fork, exec, wait */
}

/* TODO: Implement find_executable function */
char *find_executable(char *cmd) {
    /* TODO: Search for executable in path directories */
    return NULL;
}

/* TODO: Implement parse_command function */
void parse_command(char *line, char **args, int *arg_count, 
                   char **output_file, int *is_parallel) {
    /* TODO: Parse command line, handle & and > */
}

