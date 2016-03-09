#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

typedef struct burst {
    int burstNum;
    int cpuTime;
    int ioTime;
    struct burst *next;
} burst;

typedef struct thread {
    int tid;
    int arriveTime;
    int numOfBursts;
    struct burst *firstBurst;
    struct thread *next;
} thread;

typedef struct process {
    int pid;
    int numThreads;
    struct thread *firstThread;
    struct process *next;
} process;

char *getLine(FILE *fp, char *line);
void createNewProcess(FILE *fp, int id, int threadNum);
thread *createNewThread(FILE *fp, int threadID, int cpuTime, int ioTime);
thread *addThread(thread *threadToAdd, thread *threadList);
burst *createBurst(int burstNum, int cpuTime, int ioTime);
burst *addBurst(burst *burstToAdd, burst *burstList);

int main (int argc, char *argv){
    
    int numProcesses;
    int threadSwitchTime;
    int processSwitchTime;
    char buffer [500];
    process *processList;
    char fileName[100] = "inputFile.txt";
    int i;
    
    //for parsing
    //first line <how many processes, num of units to switch
    //threads in same process, num units to switch threads in different
    // processes>
    
    
    FILE* fp = fopen(fileName,"r");
    
    //get the first line in the file
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
    for (i=0;i<numProcesses;i++){
        
    }
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
    char *tempLine;
    int i = 0;
    char *token;
    int threadID;
    int arriveTime;
    int numBursts;
    
    //allocate memory for the process struct
    struct process *newProcess;
    newProcess = malloc(sizeof(struct process));
    //give it vaules
    newProcess->pid = id;
    newProcess->numThreads = threadNum;
    
    //need to make the thread list
    //use the number of threads to get the right number of threads
    struct thread *threadList;//the first thread that starts the list
    struct thread *threadToAdd;
    
    //get the line of the first thread
    tempLine = getLine(fp, buffer);
    //parse the line and assigin it to the appropriate variables
    for (i=0;i<threadNum;i++){
        token = strtok(tempLine, " ");
        threadID = atol(token);
        token = strtok(NULL, " ");
        arriveTime = atol(token);
        token = strtok(NULL, " ");
        numBursts = atol(token);
        
        //creates a new thread and give the address of the new thread to threadToAdd
        threadToAdd = createNewThread(fp, threadID, arriveTime, numBursts);
        //ands the new thread to the list of threads
        threadList = addThread(threadToAdd, threadList);
        tempLine = getLine(fp,buffer);
    }
    newProcess->firstThread = threadList;
    newProcess->next = NULL;
    
}

thread *createNewThread(FILE *fp, int threadID, int arriveTime, int numBursts){
    int burstNum;
    int cpuTime;
    int ioTime;
    burst *burstToAdd;
    burst *burstList;
    char buffer[50];
    char *tempLine;
    char *token;
    int i;
    
    thread *newThread;
    newThread = malloc(sizeof(thread));
    newThread->tid = threadID;
    newThread->arriveTime = arriveTime;
    newThread->numOfBursts = numBursts;
    
    
    //get the line of the first burst
    tempLine = getLine(fp, buffer);
    //parse the line and assigin it to the appropriate variables
    for (i=0;i<numBursts;i++){
        token = strtok(tempLine, " ");
        burstNum = atol(token);
        token = strtok(NULL, " ");
        cpuTime = atol(token);
        token = strtok(NULL, " ");
        ioTime = atol(token);
        
        //create the burst
        burstToAdd = createBurst(burstNum, cpuTime, ioTime);
        //ands the new burst to the list of burst
        burstList = addBurst(burstToAdd, burstList);
        tempLine = getLine(fp,buffer);
    }
    
    newThread->firstBurst = burstList;
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

burst *createBurst(int burstNum, int cpuTime, int ioTime){
    burst *newBurst;
    newBurst = malloc(sizeof(burst));
    newBurst->burstNum = burstNum;
    newBurst->cpuTime = cpuTime;
    newBurst->ioTime = ioTime;
    newBurst->next = NULL;
    return newBurst;
}

burst *addBurst(burst *burstToAdd, burst *burstList){
    if (burstList == NULL)
        burstList = burstToAdd;
    else {
        //find the last item in the list
        
        int check = 0;
        while (check==0){
            burst *currentBurst  = burstList;
            if (currenturst->next == NULL){
                check = 1;
                currentBurst->next = burstToAdd;
                
            } else 
                currentBurst = currentBurst->next;    
        }
    }
    return burstList;
}
