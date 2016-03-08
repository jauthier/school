#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>




typedef struct thread {
    int tid;
    int cpuTime;
    int ioTime;
    struct thread *next;
} thread;

typedef struct process {
    int pid;
    int numThreads;
    struct thread firstThread;
    struct process *next;
} process;

char *getLine(FILE *fp, char *line);
void createNewProcess(FILE *fp, int id, int threadNum);
thread *createNewThread(int threadID, int cpuTime, int ioTime);
thread *addThread(thread *threadToAdd, thread *threadList);


int main (int argc, char *argv){
    
    int numProcesses;
    int threadSwitchTime;
    int processSwitchTime;
    char buffer [500];
    process *processList;
    
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
    
    //get second line, process number and number of threads
    char *processInfo = getLine(fp, buffer);
    token = strtok(processInfo, " ");
    int processNum = atol(token);
    token = strtok(NULL, " ");
    int threadNum = atol(token);
    
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

void createNewProcess(FILE *fp, int id, int threadNum){
    
    char buffer[100];
    struct process *newProcess;
    newProcess = malloc(sizeof(struct process));
    newProcess->pid = id;
    newProcess->numThreads = threadNum;
    
    //need to make the thread list
    //use the number of threads to get the next n lines and add them to the list
    struct thread *threadList;
    struct thread *threadToAdd;
    
    //get the info if the first thread
    char *tempLine = getLine(fp, buffer);
    int i = 0;
    char *token;
    int threadID;
    int cpuTime;
    int ioTime;
    
    for (i=0;i>threadNum;i++){
        token = strtok(tempLine, " ");
        threadID = atol(token);
        token = strtok(NULL, " ");
        cpuTime = atol(token);
        token = strtok(NULL, " ");
        ioTime = atol(token);
        threadToAdd = createNewThread(threadID, cpuTime, ioTime);
        threadList = addThread(threadToAdd, threadList);
        
    }
    
    
}

thread *createNewThread(int threadID, int cpuTime, int ioTime){
    thread *newThread;
    newThread = malloc(sizeof(thread));
    newThread->tid = threadID;
    newThread->cpuTime = cpuTime;
    newThread->ioTime = ioTime;
    newThread->next = NULL;
    
    return newThread;
}

thread *addThread(thread *threadToAdd, thread *threadList){
    if (threadList == NULL)
        threadList = threadToAdd;
    else {
        //find the last item in the list
        
        int check = 0;
        while (check==0){
            thread *currentThread  = threadList;
            if (currentThread->next == NULL){
                check = 1;
                currentThread->next = threadToAdd;
                
            } else 
                currentThread = currentThread->next;    
        }
    }
    return threadList;
}

