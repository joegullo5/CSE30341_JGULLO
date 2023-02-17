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
void start_program(char* myargs[]);
void wait_for_child();
void wait_for_specific_child(int child_pid);
void run_program(char* myargs[]);

int main() {
    
}

// Exit the shell immediately
void exit_shell() {

}

// Starts another program with command line arguments
void start_command(char **args) {

}

// Wait for any child process to exit
void wait_command() {

}

// Wait for a specific child process to exit
void waitfor_command(int child_pid) {

}

// Start the program and wait for it to complete (start+waitfor)
void run_program(char* myargs[]) {

}
