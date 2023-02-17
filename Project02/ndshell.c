#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

// Constant to store max length of input Argument
#define MAX_INPUT_LEN 512
// Constant to store max number of arguments
#define MAX_ARGS 16

// Function declarations
void exit_shell();
void start_command(char* myargs[]);
void wait_command();
void waitfor_command(int child_pid);
void run_program(char* myargs[]);

int main() {
    char input[MAX_INPUT_LEN];
    char *args[MAX_ARGS];
    int should_run = 1;

    while (should_run) {
        printf("ndshell> ");
        fgets(input, MAX_INPUT_LEN, stdin);

        // remove newline character
        input[strcspn(input, "\n")] = 0;

        // tokenize the input string
        int i = 0;
        args[i] = strtok(input, " ");

        while (args[i] != NULL) {
            i++;
            args[i] = strtok(NULL, " ");
        }

        if (args[0] == NULL) {
            continue;
        }

        if (strcmp(args[0], "exit") == 0) {
            exit_shell();
        }
    }
    return 0;
}

// Exit the shell immediately
void exit_shell() {
    printf("exiting shell immediately\n");
    exit(0);
}

// Starts another program with command line arguments
void start_command(char **args) {

}

// Wait for any child process to exit
void wait_command() {

}

void waitfor_command(int child_pid) {

}

// Start the program and wait for it to complete (start+waitfor)
void run_command(char* myargs[]) {

}
