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


void termination_handle(int signum) {
    printf("\nControl-C was pressed .. exiting\n");
    exit(0);
}

int main() {

    // prompt the user for input
    printf("Execute? ");
    fflush(stdout);  // flush output buffer to ensure prompt is displayed immediately

    // read the user's input
    char input[1024];
    fgets(input, 1024, stdin);

    // remove trailing newline character, if present
    int len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }

    // TODO: process the user's input




    return 0;
}

