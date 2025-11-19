#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stddef.h> 
#include <limits.h>


#define MAX_LINE_LENGTH 512 
#define MAX_ARGS 64
#define MAX_PATH_DIRS 64

/* Global variables */
char* path_dirs[MAX_PATH_DIRS];
int path_count = 0;

/* Function declarations */
void print_error(void);
void initialize_path(void);
void set_path(char** args, int arg_count);
int is_builtin(char* cmd);
void execute_builtin(char** args, int arg_count);
void execute_command(char** args, int arg_count, char* output_file);
char* find_executable(char* cmd);
int parse_command(char* line, char** args, int* arg_count,
    char** output_file);
void free_path(void);
void print_welcome(void);
void clean_args_memory(char** args, int count);
void clean_token(char* s);


int main(int argc, char* argv[]) {
    FILE* input;
    char* line = NULL;
    size_t len = 0;
    ssize_t read;
    int interactive = 1;
    char* args[MAX_ARGS];
    int arg_count;
    char* output_file = NULL;
    pid_t children[MAX_ARGS];
    int child_count;
    int i;
    int status;
    int parse_status;

    /* Set default path */
    initialize_path();

    /* Check input source */
    if (argc > 2) {
        print_error();
        free_path();
        exit(1);
    }

    if (argc == 2) {
        /* Batch mode */
        char* fname = argv[1];
        size_t flen = strlen(fname);
        while (flen > 0 && (fname[flen - 1] == '\r' ||
            fname[flen - 1] == '\n' ||
            fname[flen - 1] == ' ' ||
            fname[flen - 1] == '\t')) {
            fname[--flen] = '\0';
        }

        input = fopen(fname, "r");
        if (input == NULL) {
            print_error();
            free_path();
            return 1;
        }
        interactive = 0;
    }
    else {
        input = stdin;
        interactive = isatty(fileno(stdin));
        if (interactive) {
            print_welcome();
            fprintf(stderr, "[INTERACTIVE MODE]\n");
        }
    }

    /* Main loop */
    while (1) {
        if (interactive) {
            printf("wish> ");
            fflush(stdout);
        }

        read = getline(&line, &len, input);

        if (read == -1) {
            /* EOF or error */
            if (line) {
                free(line);
                line = NULL;
            }
            if (input != stdin) {
                fclose(input);
            }
            free_path();
            exit(0);
        }

        /* Remove newline and carriage return characters */
        if (read > 0 && line[read - 1] == '\n') {
            line[read - 1] = '\0';
            read--;
        }
        if (read > 0 && line[read - 1] == '\r') {
            line[read - 1] = '\0';
            read--;
        }

        /* Ignore empty lines or lines with only whitespace */
        if (read <= 0 || (size_t)strspn(line, " \t") == (size_t)read) {
            continue;
        }

        // Check for comments before parsing
        if (line[strspn(line, " \t")] == '#') {
            continue;
        }


        /* Parse parallel commands */
        child_count = 0;
        char* saveptr;
        char* line_copy = strdup(line);
        if (line_copy == NULL) {
            continue;
        }
        char* parallel_cmd = strtok_r(line_copy, "&", &saveptr);

        while (parallel_cmd != NULL && child_count < MAX_ARGS) {
            // Trim leading whitespace from the parallel command part
            while (*parallel_cmd == ' ' || *parallel_cmd == '\t') {
                parallel_cmd++;
            }

            if (strlen(parallel_cmd) == 0) {
                parallel_cmd = strtok_r(NULL, "&", &saveptr);
                continue;
            }

            arg_count = 0;
            output_file = NULL;

            // Ensure args array is clean before parsing
            clean_args_memory(args, 0);

            // Call parse_command with updated signature
            parse_status = parse_command(parallel_cmd, args, &arg_count, &output_file);

            if (parse_status == -1) {
                // print_error() was called in parse_command.
                if (output_file) {
                    free(output_file);
                    output_file = NULL;
                }
                clean_args_memory(args, arg_count);

                parallel_cmd = strtok_r(NULL, "&", &saveptr);
                continue;
            }

            if (arg_count == 0) {
                // Command was empty
                if (output_file) {
                    free(output_file);
                    output_file = NULL;
                }
                parallel_cmd = strtok_r(NULL, "&", &saveptr);
                continue;
            }

            /* Check for built-in commands */
            if (is_builtin(args[0])) {
                // Built-in commands cannot have redirection
                if (output_file != NULL) {
                    print_error();
                }
                else {
                    execute_builtin(args, arg_count);
                }
            }
            else {
                // External command: Create child process
                pid_t pid = fork();

                if (pid < 0) {
                    print_error();
                }
                else if (pid == 0) {
                    /* Child process */
                    execute_command(args, arg_count, output_file);
                    exit(0);
                }
                else {
                    /* Parent process */
                    children[child_count++] = pid;
                }
            }

            /* Clean up resources for this command */
            if (output_file) {
                free(output_file);
                output_file = NULL;
            }
            clean_args_memory(args, arg_count);

            parallel_cmd = strtok_r(NULL, "&", &saveptr);
        }

        free(line_copy);

        /* Wait for all child processes to finish */
        for (i = 0; i < child_count; i++) {
            waitpid(children[i], &status, 0);
        }
    }

    /* Exit cleanup (unreachable) */
    if (line) {
        free(line);
    }
    if (input != stdin) {
        fclose(input);
    }
    free_path();

    return 0;
}

/**
 * Prints the generic error message to stderr
 */
void print_error(void) {
    // Corrected to standard error message without '!!!'
    char error_message[] = "An error has occurred\n";
    write(STDERR_FILENO, error_message, sizeof(error_message) - 1);
}

/**
 * Cleans up dynamically allocated memory in the args array
 */
void clean_args_memory(char** args, int count) {
    int i;
    for (i = 0; i < count; i++) {
        if (args[i]) {
            free(args[i]);
            args[i] = NULL;
        }
    }
}


/**
 * Prints welcome message (for interactive mode)
 */
void print_welcome(void) {
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                                                              ║\n");
    printf("║      PAKISTAN - CHINA    UNIX SHELL                          ║\n");
    printf("║                                                              ║\n");
    printf("║      Welcome to wish - A Simple Unix Shell                   ║\n");
    printf("║                                                              ║\n");
    printf("║      \"Creaters Aina Zainab & Wang Dedian\"                    ║\n");
    printf("║                                                              ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    printf("\n");
}

/**
 * Initializes the default PATH directories
 */
void initialize_path(void) {
    free_path();
    path_count = 0;

    char* env_path = getenv("PATH");
    if (env_path == NULL || *env_path == '\0') {
        path_dirs[path_count++] = strdup("/bin");
        path_dirs[path_count] = NULL;
        return;
    }

    char* copy = strdup(env_path);
    if (copy == NULL) {
        path_dirs[path_count++] = strdup("/bin");
        path_dirs[path_count] = NULL;
        return;
    }

    char* saveptr = NULL;
    char* token = strtok_r(copy, ":", &saveptr);
    while (token != NULL && path_count < MAX_PATH_DIRS - 1) {
        if (strlen(token) > 0) {
            path_dirs[path_count++] = strdup(token);
        }
        token = strtok_r(NULL, ":", &saveptr);
    }

    path_dirs[path_count] = NULL;
    free(copy);
}

void free_path(void) {
    int i;
    for (i = 0; i < path_count; i++) {
        if (path_dirs[i]) {
            free(path_dirs[i]);
            path_dirs[i] = NULL;
        }
    }
    path_count = 0;
}

/**
 * FIXED: Cleans leading and trailing whitespace (including \r, \n)
 * from a string *in-place*. Uses memmove to shift content.
 */
void clean_token(char* s) {
    if (s == NULL || *s == '\0') return;

    // 1. Clean trailing whitespace
    char* end = s + strlen(s) - 1;
    while (end >= s && (*end == ' ' || *end == '\t' || *end == '\r' || *end == '\n')) {
        *end = '\0';
        end--;
    }

    // 2. Handle leading whitespace
    char* start = s;
    while (*start == ' ' || *start == '\t') {
        start++;
    }

    // If content was shifted, move it back to the buffer head
    if (start != s) {
        if (*start != '\0') {
            memmove(s, start, strlen(start) + 1);
        }
        else {
            *s = '\0'; // If only spaces, make it an empty string
        }
    }
}


/**
 * Checks if a command is a built-in command
 */
int is_builtin(char* cmd) {
    if (cmd == NULL) return 0;
    return (strcmp(cmd, "exit") == 0 ||
        strcmp(cmd, "cd") == 0 ||
        strcmp(cmd, "path") == 0);
}


/**
 * Executes a built-in command
 */
void execute_builtin(char** args, int arg_count) {
    if (args[0] == NULL) return;

    if (strcmp(args[0], "exit") == 0) {
        if (arg_count > 1) {
            print_error();
            return;
        }
        free_path();
        exit(0);
    }
    else if (strcmp(args[0], "cd") == 0) {
        if (arg_count != 2) {
            print_error();
            return;
        }
        if (chdir(args[1]) != 0) {
            print_error();
        }
    }
    else if (strcmp(args[0], "path") == 0) {
        set_path(args, arg_count);
    }
}

/**
 * Sets the new execution PATH
 */
void set_path(char** args, int arg_count) {
    int i;
    free_path();
    path_count = 0;

    if (arg_count == 1) {
        path_dirs[0] = NULL;
        return;
    }
    for (i = 1; i < arg_count && path_count < MAX_PATH_DIRS - 1; i++) {
        // Ensure path directory is not empty/whitespace
        if (strlen(args[i]) > 0 && strspn(args[i], " \t") != strlen(args[i])) {
            path_dirs[path_count++] = strdup(args[i]);
        }
    }
    path_dirs[path_count] = NULL;
}

/**
 * Child process executes an external command
 */
void execute_command(char** args, int arg_count, char* output_file) {
    int fd;
    char* executable;

    if (arg_count == 0 || args[0] == NULL) {
        print_error();
        exit(1);
    }

    executable = find_executable(args[0]);
    if (executable == NULL) {
        print_error();
        exit(1);
    }

    /* Handle redirection */
    if (output_file != NULL) {
        fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0) {
            print_error();
            free(executable);
            exit(1);
        }

        /* Redirect STDOUT only. STDERR remains untouched. */
        if (dup2(fd, STDOUT_FILENO) == -1) {
            print_error();
            close(fd);
            free(executable);
            exit(1);
        }
        close(fd);
    }

    execv(executable, args);

    int saved_errno = errno;

    if (saved_errno == EACCES || saved_errno == ENOEXEC) {
        int new_argc = arg_count + 1;
        char** sh_argv = malloc((new_argc + 1) * sizeof(char*));
        if (sh_argv != NULL) {
            sh_argv[0] = "/bin/sh";
            sh_argv[1] = executable;
            for (int i = 1; i < arg_count; i++) {
                sh_argv[i + 1] = args[i];
            }
            sh_argv[new_argc] = NULL;

            execv("/bin/sh", sh_argv);
        }
    }

    print_error();
    free(executable);
    exit(1);
}


/**
 * Searches for the executable file in the directories specified by PATH.
 * Uses a copy of cmd for cleaning.
 * Returns a dynamically allocated string with the full path, or NULL if not found.
 */
char* find_executable(char* cmd) {
    if (cmd == NULL) return NULL;

    char* cmd_copy = strdup(cmd);
    if (cmd_copy == NULL) {
        return NULL;
    }

    clean_token(cmd_copy);

    if (*cmd_copy == '\0' || path_count == 0) {
        free(cmd_copy);
        return NULL;
    }

    for (int i = 0; i < path_count; i++) {
        if (path_dirs[i] == NULL) continue;

        size_t len_dir = strlen(path_dirs[i]);
        size_t len_cmd = strlen(cmd_copy);

        /* Allocate space: dir + "/" + cmd + '\0' */
        char* full = malloc(len_dir + 1 + len_cmd + 1);
        if (full == NULL) {
            continue;
        }

        strcpy(full, path_dirs[i]);
        if (len_dir > 0 && full[len_dir - 1] != '/') {
            strcat(full, "/");
        }
        strcat(full, cmd_copy);
        if (access(full, X_OK) == 0 || access(full, F_OK) == 0) {
            free(cmd_copy);
            return full;
        }

        free(full);
    }

    free(cmd_copy);
    return NULL;
}


int parse_command(char* line, char** args, int* arg_count,
    char** output_file)
{
    *arg_count = 0;
    *output_file = NULL;

    if (line == NULL) return 0;

    char* copy = strdup(line);
    if (copy == NULL) {
        print_error();
        return -1;
    }

    char* cmd_part = copy;
    char* redir_part = NULL;
    char* gt_count = NULL;

    /* 查找重定向符号 '>' */
    gt_count = strchr(copy, '>');
    if (gt_count != NULL) {
        /* 检查是否有多个 '>'：比如 cmd > f1 > f2 是非法的 */
        if (strchr(gt_count + 1, '>') != NULL) {
            print_error();
            free(copy);
            return -1;
        }

        /* 切分出命令部分和重定向部分 */
        *gt_count = '\0';        /* 结束命令部分 */
        redir_part = gt_count + 1;

        /* ---- 关键新增：检测 “> file” 这种前面没有命令的情况 ---- */
        /* 如果命令部分只有空白（或完全为空），这是非法重定向 */
        if (cmd_part == NULL ||
            strspn(cmd_part, " \t\n\r") == strlen(cmd_part)) {
            print_error();
            free(copy);
            return -1;
        }
        /* ---------------------------------------------------------------- */

        /* --- 解析文件名 --- */
        char* filename_start = redir_part;
        while (*filename_start == ' ' || *filename_start == '\t')
            filename_start++;

        /* '>   ' 没有文件名，也是错误 */
        if (*filename_start == '\0') {
            print_error();
            free(copy);
            return -1;
        }

        char* filename_end = filename_start;
        while (*filename_end && *filename_end != ' ' && *filename_end != '\t'
            && *filename_end != '\n' && *filename_end != '\r')
            filename_end++;

        size_t len = filename_end - filename_start;
        *output_file = malloc(len + 1);
        if (*output_file == NULL) {
            print_error();
            free(copy);
            return -1;
        }
        strncpy(*output_file, filename_start, len);
        (*output_file)[len] = '\0';

        /* 文件名后面如果还有非空白字符，也是错误：比如 'cmd > file extra' */
        char* extra_check = filename_end;
        while (*extra_check == ' ' || *extra_check == '\t'
            || *extra_check == '\n' || *extra_check == '\r') {
            extra_check++;
        }

        if (*extra_check != '\0') {
            print_error();
            free(*output_file);
            *output_file = NULL;
            free(copy);
            return -1;
        }
    }

    /* ------------ 解析命令部分 -------------- */
    /* 用空格/Tab/换行/回车分割 token */
    char* token = strtok(cmd_part, " \t\n\r");
    while (token != NULL && *arg_count < MAX_ARGS - 1) {

        if (strlen(token) > 0) {

            /* 清理 token 的前后空白，然后复制 */
            clean_token(token);

            args[*arg_count] = strdup(token);
            if (args[*arg_count] == NULL) {
                print_error();
                clean_args_memory(args, *arg_count);
                if (*output_file) free(*output_file);
                *output_file = NULL;
                free(copy);
                return -1;
            }
            (*arg_count)++;
        }

        token = strtok(NULL, " \t\n\r");
    }

    args[*arg_count] = NULL;   /* 以 NULL 结尾 */

    free(copy);
    return 0;
}
