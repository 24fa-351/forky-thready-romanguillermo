#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

void pattern1_parent(int num_processes);
void pattern1_child(int process_num);
void pattern2_parent(int num_processes);
void pattern2_child(int id, int process_num);

int main(int argc, char *argv[]) {
    int num_processes = atoi(argv[1]);
    int pattern = atoi(argv[2]);

    if (num_processes < 1 || num_processes > 256) {
        fprintf(stderr, "Error: Number of things must be between 1 and 256.\n");
        exit(EXIT_FAILURE);
    }

    if (pattern == 1) {
        printf("Pattern 1, %d processes\n", num_processes);
        pattern1_parent(num_processes);
    } else if (pattern == 2) {
        printf("Pattern 2, %d processes\n", num_processes);
        pattern2_parent(num_processes);
    } else {
        fprintf(stderr, "Error: Pattern number must be 1 or 2.\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}

void pattern1_parent(int num_processes) {
    pid_t pids[num_processes];

    // Fork all child processes
    for (int i = 0; i < num_processes; i++) {
        pids[i] = fork();
        if (pids[i] < 0) {
            perror("Fork failed");
            exit(1);
        } else if (pids[i] == 0) {  // Child process
            pattern1_child(i + 1);
            exit(0);
        } else {
            printf("Main process (%d) started Process %d (%d)\n", getpid(), i + 1, pids[i]);
        }
    }

    // Wait for all child processes
    for (int i = 0; i < num_processes; i++) {
        waitpid(pids[i], NULL, 0);
    }

    printf("Main process (%d) exiting.\n", getpid());
}

void pattern1_child(int id) {
    srand(time(NULL));
    int sleep_time = rand() % 8 + 1;
    printf("Process %d (%d) beginning.\n", id, getpid());
    sleep(sleep_time);
    printf("Process %d (%d) exiting.\n", id, getpid());
}


void pattern2_parent(int num_processes) {
    int id = 1;
    pid_t pid;

    printf("Process %d (%d) beginning.\n", id, getpid());

    if (id < num_processes) {
        pid = fork();
        if (pid < 0) {
            perror("Fork failed");
            exit(1);
        }
        else if (pid > 0) {  // Parent process
            // Parent knows the child's PID here
            printf("Process %d (%d) creating Process %d (%d)\n", id, getpid(), id + 1, pid);
            wait(NULL);  // Wait for the child to complete
            printf("Process %d (%d) exiting.\n", id, getpid());
            exit(0);  // Parent exits after handling its child
        }
        else {  // Child process
            pattern2_child(id + 1, num_processes);
        }
    }
    else {  // If id == num_processes, perform sleep and exit
        srand(time(NULL));
        int sleep_time = rand() % 8 + 1;
        sleep(sleep_time);
        printf("Process %d (%d) exiting.\n", id, getpid());
    }
}

void pattern2_child(int id, int num_processes) {
    printf("Process %d (%d) beginning.\n", id, getpid());

    if (id < num_processes) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("Fork failed");
            exit(1);
        }
        else if (pid > 0) {  // Parent process
            // Parent knows the child's PID here
            printf("Process %d (%d) creating Process %d (%d)\n", id, getpid(), id + 1, pid);
            wait(NULL);  // Wait for the child to complete
            printf("Process %d (%d) exiting.\n", id, getpid());
            exit(0);  // Parent exits after handling its child
        }
        else {  // Child process
            pattern2_child(id + 1, num_processes);
        }
    }
    else {
        srand(time(NULL));
        int sleep_time = rand() % 8 + 1;
        sleep(sleep_time);
        printf("Process %d (%d) exiting.\n", id, getpid());
    }
}