#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <sys/wait.h>

// Constant to store max length of input Argument
#define MAX_INPUT_LEN 1000
// Constant to store max number of arguments
#define MAX_ARGS 100

//Struct to store process information. This is a doubly linked list
typedef struct node {
    int pid;
    time_t created_time;
    char name[20];
    struct node* next;
    struct node* prev;
} Node;

Node* head = NULL;
Node* tail = NULL;


int num_processes = 0;
void addNode(int pid, char name[]);
void removeNode(int pid);
void display();
// Function declarations
void exit_shell();
void start_command(char* myargs[]);
void wait_command();
void waitfor_command(int child_pid);
void run_command(char* myargs[]);
void kill_command(int child_pid);
void quit_command();
void ps(Node* node);
void display(Node* curr);

// Function to handle interupt and terminate program.
void termination_handler(int signum) {
    signal(SIGINT, termination_handler);
    printf("\n");
    kill_command(tail->pid);
    removeNode(tail->pid);
    fflush(stdout);
}

int main() {
    char input[MAX_INPUT_LEN];
    char *words[MAX_ARGS];
    int should_run = 1;
    pid_t child_pid;

    // setup signal handler for SIGINT (Ctrl-C) -- Based off of Code from lecture 3 slides
    signal(SIGINT, termination_handler);

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
        } else if (strcmp(words[0], "kill") == 0) {
            if (words[1] == NULL) {
                printf("ndshell: No process ID provided.\n");
                continue;
            }
            child_pid = atoi(words[1]);
            kill_command(child_pid);
        } else if (strcmp(words[0], "ps") == 0) {
            printf("== Process List (%d known processes) ==\n",num_processes);
            printf("PID\tCreated Time\t\t\tName\n");
            ps(head);
        } else if (strcmp(words[0], "quit") == 0) {
            quit_command();
        }
    }
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
            return;
        }
    } else {
        printf("ndshell: process %d started\n", pid);
        addNode(pid, args[1]);
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


void kill_command(int child_pid) {

    // send SIGKILL signal to child process
    int kill_result = kill(child_pid, SIGKILL);
    if (kill_result == -1) {
        perror("ndshell: kill failed");
        return;
    }

    // wait for child process to exit
    int status;
    pid_t result = waitpid(child_pid, &status, 0);

    if (result == -1) {
        perror("ndshell: waitpid failed");
        return;
    }

    // check exit status
    if (WIFEXITED(status)) {
        printf("ndshell: process %d exited with status %d\n", child_pid, WEXITSTATUS(status));
        num_processes--;
    } else if (WIFSIGNALED(status)) {
        printf("ndshell: process %d exited abnormally with signal %d: %s\n", child_pid, WTERMSIG(status), strsignal(WTERMSIG(status)));
        num_processes--;
    }
}




void addNode(int pid, char name[]) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    num_processes++;
    newNode->pid = pid;
    newNode->created_time = time(NULL);
    strcpy(newNode->name, name);
    newNode->next = NULL;
    newNode->prev = NULL;
    if (head == NULL) {
        head = tail = newNode;
        return;
    }

    tail->next = newNode;
    newNode->prev = tail;
    tail = newNode;
}

void removeNode(int pid) {
    Node* curr = head;
    while (curr != NULL && curr->pid != pid) {
        curr = curr->next;
    }

    if (curr == NULL) {
        num_processes--;
        return;
    }

    if (curr == head && curr == tail) {
        head = tail = NULL;
    }
    else if (curr == head) {
        head = curr->next;
        head->prev = NULL;
    }
    else if (curr == tail) {
        tail = curr->prev;
        tail->next = NULL;
    }
    else {
        curr->prev->next = curr->next;
        curr->next->prev = curr->prev;
    }
    num_processes--;
    free(curr);
}

void ps(Node* node) {

    if (node != NULL)
    {
        int status;
        pid_t result = waitpid(node->pid, &status, WNOHANG);
        if (result == 0) {
            // child process still running
            display(node);
            if (node->next != NULL)
            {
                ps(node->next);
            }
        } else if (result == -1) {
            return;
        } else {
            removeNode(node->pid);  // remove the process from the linked list
            ps(head); //call ps on head instead
        }
    }
}

//Helper function for ps. Displays node infromation
void display(Node* curr) {
    char *t = ctime(&curr->created_time);
    t[strlen(t)-1] = '\0';
    printf("%d\t%s\t%s\n", curr->pid, t, curr->name);
}

// Kills each active process in the node list and quits the program.
void quit_command()
{
    Node* node = head;
    int status;
    while(node != NULL)
    {
        if(waitpid(node->pid, &status, WNOHANG) == 0)
        {
            kill_command(node->pid);
        } 
        node = node->next;
    }
    printf("All child processes complete – exiting the shell.\n");
    exit(0);
}


