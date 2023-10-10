#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    //So probably the best way to determine k would be to have a variable that decrements to let us know at which stage of the fork we're on
    //From there, do we want to pass a message via argv or have a scanf input for stuff??

    //in either case, we would need to pass the message along, along with a number to let the message know what process it was intended for (struct, maybe?)
    //This actually doesn't seem too bad

    //from here, each node needs to be able to read data from it's parent node and write to child node. Do we also want to be able to scanf in the parent node?
    //So yeah, based on the specs, it sounds like scanf is going to be required for the specs here

    //also going to need a catchment to make sure that argv is 2 (or maybe more if we want to be able to pass strings in it)
    //But let's hold off on that until we've got a little more code

    // if (argc < 2) {
    //     perror("Too few arguments to run this thing\n");
    //     return 0;
    // }

    //the k in our k processes
    int K = 10; //change this to argv[1] later. Just making this a constant for now so we have to change it in less places the further in we get.

    pid_t child_pids[K]; //change 5 to atdi(argv[1]) before submitting. But this will just be to test;

    int fd[2]; //will need this...maybe

    int pipes[K][2];

    for (int i = 0; i < K; i++) {
    if (pipe(pipes[i]) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Child process
        close(pipes[i][1]); // Close write end of the pipe

        // Read data from the parent and write to the child's next process
        // Handle the read and write operations within the child process

        // Example:
        //char buffer[1024];
        char buffer[1000];
        ssize_t nbytes = read(pipes[i][0], buffer, sizeof(buffer));
        if (nbytes == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        // Process data if needed

        // Write data to the next child process
        if (i < K - 1) {
            if (i) {
                buffer[0] = 'Q';
            }

            if (write(pipes[i + 1][1], buffer, nbytes) == -1) {
                perror("write");
                exit(EXIT_FAILURE);
            }
        } else {
            // If this is the last child, write back to the parent
            if (write(pipes[0][1], buffer, nbytes) == -1) {
                perror("write");
                exit(EXIT_FAILURE);
            }
        }

        close(pipes[i][0]); // Close read end of the pipe
        exit(EXIT_SUCCESS);
    } else {
        // Parent process
        close(pipes[i][0]); // Close read end of the pipe
        child_pids[i] = pid;
    }
}

    char initialD[] = "Banana\n";
    if (write(pipes[0][1], initialD, sizeof(initialD)) == -1) {
        perror("Failed to write initial data");
        exit(1);
    }

    for (int i = 0; i < K; i++) {
        close(pipes[i][1]);
    }

    for (int i = 0; i < K; i++) {
        waitpid(child_pids[i], NULL, 0);
    }

    return 0;
}
