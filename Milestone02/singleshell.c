/*
    Author - Joseph Gullo
    Course CSE 30341
    Date: 2/09/2023
    Description: 
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

// Constant to store max length of input Argument
#define MAX_INPUT_LEN 512
// Constant to store max numnber of arguments
#define MAX_ARGS 16

// Function to handle interupt and terminate program.
void termination_handler(int signum) {
    printf("\nControl-C was pressed .. exiting\n");
    exit(0);
}

int main() {

    // setup signal handler for SIGINT (Ctrl-C) -- Based off of Code from lecture 3 slides
    struct sigaction new_action, old_action;

    new_action.sa_handler = termination_handler;

    sigemptyset(&new_action.sa_mask);

    new_action.sa_flags = 0;

    sigaction(SIGINT, NULL, &old_action);

    if (old_action.sa_handler != SIG_IGN) {
        sigaction(SIGINT, &new_action, NULL);
    }

    // prompt the user for input
    printf("Execute? ");

    // read the user's input into the input array
    char input[MAX_INPUT_LEN];
    fgets(input, MAX_INPUT_LEN, stdin);

    // remove trailing newline character if there is one
    int len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }

    // parse the user's input into a command and its arguments using the strtok function.
    // This function moves through the user inputs and "chops it up" into tokens that are each separate strings in an array.
    char* myargs[MAX_ARGS];
    int i = 0;
    char* token = strtok(input, " ");
    while (token != NULL) {
        myargs[i++] = token; // stores token into next space in myargs
        token = strtok(NULL, " "); //returns null if there are no arguments left
    }

    myargs[i] = NULL;

    // fork child process
    int rc = fork();

    if (rc < 0) {
        fprintf(stderr, "Error: Failed to fork\n");
        exit(1);
    } else if (rc == 0) {
        // Print the command and its arguments
        printf("Executing: %s", myargs[0]);
        for (int i = 1; myargs[i] != NULL; i++) {
            printf(" %s", myargs[i]);
        }
        printf("\n");

        // Child process -- execute the command
        execvp(myargs[0], myargs);

        // Returns if there's an error
        fprintf(stderr, "Error: Failed to execute command. Please enusre this command exists.\n");
        exit(1);
    } else {
        // Parent process -- wait for child to complete
        int status;
        wait(&status);

        //Checks to make sure child exited with a non-zero status code (i.e. didn't terminate abnormally)
        if (WIFEXITED(status)) {
            printf("Execution complete!\n");
        } else {
            fprintf(stderr,"Error: Child process terminated abnormally\n");
        }
    }

    return 0;
}


