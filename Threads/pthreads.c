#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>


char *getLine(FILE *fp, char *line);

struct thread {
    int tid;
    int cpuTime;
    int ioTime;
    struct thread *next;
} thread;

struct Process {
    int pid;
    int numThreads;
    struct thread firstThread;
} process;



int main (int argc, char *argv){
    
    int numProcesses;
    int threadSwitchTime;
    int processSwitchTime;
    char buffer [500];
    
    //for parsing
    //first line <how many processes, num of units to switch
    //threads in same process, num units to switch threads in different
    // processes>
    
    char fileName[100] = "inputFile.txt";
    
    FILE* fp = fopen(fileName,"r");
    
    //get the first line of the file
    char *firstline = getLine(fp, buffer);
    //parse the first line
    char *token = strtok(firstline," ");
    numProcesses = atol(token);
    token = strtok(NULL, " ");
    threadSwitchTime = atol(token);
    token = strtok(NULL, " ");
    processSwitchTime = atol(token);
    
    printf("there are %d processes, %d, %d.\n", numProcesses, threadSwitchTime, processSwitchTime);
    
    
    return 0;
}

char* getLine(FILE* fp, char *line){
    
    if (fp == NULL) {
        printf("Unable to open file.\n");
        exit(0);
    }
    
    int maxLen = 20;
    char *lineBuffer = malloc(sizeof(char)*maxLen);
    
    int count = 0;
    char c;
    c = fgetc(fp);
    while (c != '\n' && c != EOF){
        lineBuffer[count] = c;
        count ++;
        c = fgetc(fp);
    }
    
    lineBuffer[count] = '\0'; 

    
    stpcpy(line, lineBuffer);
    free(lineBuffer);
   
    return line;
}



