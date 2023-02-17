#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

// Constant to store max length of input Argument
#define MAX_INPUT_LEN 1000
// Constant to store max number of arguments
#define MAX_ARGS 100

// Function declarations
void exit_shell();
void start_command(char* myargs[]);
void wait_command();
void waitfor_command(int child_pid);
void run_command(char* myargs[]);

int main() {
    char input[MAX_INPUT_LEN];
    char *words[MAX_ARGS];
    int should_run = 1;
    int child_pid;

    while (should_run) {
        printf("ndshell> ");
        fflush(stdout);
        fgets(input, MAX_INPUT_LEN, stdin);

        // remove newline character
        input[strcspn(input, "\n")] = 0;

        // tokenize the input string
        int i = 0;
        words[i] = strtok(input, " ");

        while (words[i] != NULL) {
            i++;
            words[i] = strtok(NULL, " ");
        }

        if (words[0] == NULL) {
            continue;
        }

        if (strcmp(words[0], "exit") == 0) {
            exit_shell();
        } else if (strcmp(words[0], "start") == 0) {
            start_command(words);
        } else if (strcmp(words[0], "wait") == 0) {
            wait_command();
        } else if (strcmp(words[0], "waitfor") == 0) {
            if (words[1] == NULL) {
                printf("ndshell: No process ID provided.\n");
                continue;
            }
            child_pid = atoi(words[1]);
            waitfor_command(child_pid);
        } else if (strcmp(words[0], "run") == 0) {
            run_command(words);
        }
    }

    return 0;
}




// Exit the shell immediately
void exit_shell() {
    printf("Exiting shell immediately\n");
    exit(0);
}

// Starts another program with command line arguments
void start_command(char **args) {
    pid_t pid = fork();

    if (pid < 0) {
        fprintf(stderr, "Fork failed\n");
        return;
    } else if (pid == 0) {
        if (execvp(args[1], &args[1]) < 0) {
            fprintf(stderr, "Execution failed\n");
            exit(1);
        }
        exit(0);
    } else {
        printf("ndshell: process %d started\n", pid);
        return;
    }
}



// Wait for any child process to exit
void wait_command() {
    int status;

    pid_t pid = wait(&status);

    if (pid == -1) {
        printf("ndshell: No children.\n");
        return;
    } else {
        if (WIFEXITED(status)) {
            printf("ndshell: process %d exited normally with status %d\n", pid, WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("ndshell: process %d exited abnormally with signal %d: %s\n", pid, WTERMSIG(status), strsignal(WTERMSIG(status)));
        }
    }

}

void waitfor_command(int child_pid) {
    int status;
    int child_status;

    // Wait for specific child process to exit
    pid_t pid = waitpid(child_pid, &status, 0);

    // Check if child process exists
    if (pid == -1) {
        printf("ndshell: No such process.\n");
        return;
    }

    // Check if child process exited normally or abnormally
    if (WIFEXITED(status)) {
        child_status = WEXITSTATUS(status);
        printf("ndshell: process %d exited normally with status %d\n", pid, child_status);
    } else if (WIFSIGNALED(status)) {
        child_status = WTERMSIG(status);
        printf("ndshell: process %d exited abnormally with signal %d: %s.\n", pid, child_status, strsignal(child_status));
    } else {
        printf("ndshell: process %d exited abnormally.\n", pid);
    }
}

// Start the program and wait for it to complete (start+waitfor)
void run_command(char* myargs[]) {
    pid_t pid = fork();

    if (pid < 0) {
        fprintf(stderr, "Fork failed\n");
        return;
    } else if (pid == 0) {
        if (execvp(myargs[1], &myargs[1]) < 0) {
            fprintf(stderr, "Execution failed\n");
        }
        exit(0);
    } else {
        printf("ndshell: process %d started\n", pid);
        waitfor_command(pid);
    }
}