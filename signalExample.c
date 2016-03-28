#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <ctype.h>

void endSignalRoutine(int signal, siginfo_t *signalInfo, void *hold);

int main(int argc, char** argv){
    pid_t childPID;//for forking

    if(argc < 2) {
        printf("\nUsage: enter the number of seconds for child process to sleep as a command line argument.\n\n");
        exit(-1);
    }

    //declare our signal handler.
    struct sigaction test;

    test.sa_sigaction = endSignalRoutine;//identifies the signal catching function

    childPID = fork();

    if(childPID >= 0) {
    	if(childPID == 0) {
    		//This is the child process
    		printf("I am child process. My PID: %d.\n", getpid());

            execvp("sleep", argv);

            exit(0);
    	} else {
    		//this is the parent process
    		printf("I am parent process. My PID: %d\n", getpid());

            // Registered for SIGCHLD 
            sigaction(SIGCHLD, &test, NULL);

            while(1) {
                printf("parent waiting...\n");
                sleep(1);
            }
    	}
    } else {
    	printf("Failed to fork.\n");
    }
	return 0;
}



//signal catching function
void endSignalRoutine(int signal, siginfo_t *signalInfo, void *hold)
{
    printf("Process %d completed.\n", signalInfo->si_pid);

    if(signal == SIGINT) {
        exit(0);
    }
}


