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
    int pid;
    int arriveTime;
    int numOfBursts;
    char *state;
    int endTime;
    int totalCPU;
    int totalIO;
    struct burst *firstBurst;
    struct thread *next;
} thread;

thread *loadThreads(FILE* fp);
char *getLine(FILE *fp, char *line);
thread *createNewThread(FILE *fp, int processID, int threadID, int cpuTime, int ioTime);
thread *addThread(thread *threadToAdd, thread *threadList);
burst *createBurst(int burstNum, int cpuTime, int ioTime);
burst *addBurst(burst *burstToAdd, burst *burstList);

int main (int argc, char *argv){
    
    char fileName[100] = "inputFile.txt";
    FILE* fp = fopen(fileName,"r");
    thread *threadList;
    threadList = loadThreads(fp);
    fclose(fp);
    return 0;
}

thread *loadThreads(FILE* fp){

    int numProcesses;
    int threadSwitchTime;
    int processSwitchTime;
    char buffer [50];
    thread *threadList = NULL;
    int i=0;
    char *tempLine;
    
    /* Get the first line of the input file, this will tell how many processes there are,
    the time it takes to switch beteen threads of the same process and the time it takes
    to switch between threads of different processes */
    char *firstline = getLine(fp, buffer);
    char *token = strtok(firstline," ");
    numProcesses = atol(token);
    token = strtok(NULL, " ");
    threadSwitchTime = atol(token);
    token = strtok(NULL, " ");
    processSwitchTime = atol(token);
    
    /* Make a list of threads. First we must get the line decribing the process to know
    how many threads each process has. All the threads are add to the same list. */
    for (i=0;i<numProcesses;i++){
		
        int j = 0;
        int processNum;
        int threadNum;
        int threadID;
        int arriveTime;
        int numBursts;
        struct thread *threadToAdd;
        
        //get the line
        tempLine = getLine(fp, buffer);
        //parse the line
        token = strtok(tempLine, " ");
        processNum = atol(token);
        token = strtok(NULL, " ");
        threadNum = atol(token);
        
        //get the right amount of threads
        for (j=0;j<threadNum;j++){
            char threadBuffer[50];
            tempLine = getLine(fp, threadBuffer);
            token = strtok(tempLine, " ");
            threadID = atol(token);
            token = strtok(NULL, " ");
            arriveTime = atol(token);
            token = strtok(NULL, " ");
            numBursts = atol(token);
            //creates a new thread and give the address of the new thread to threadToAdd
            threadToAdd = createNewThread(fp, processNum, threadID, arriveTime, numBursts);
            printf("one thread made\n");
            //ands the new thread to the list of threads
            threadList = addThread(threadToAdd, threadList);
        }
    }
    return threadList;
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

thread *createNewThread(FILE *fp, int processID, int threadID, int arriveTime, int numBursts){
    int burstNum;
    int cpuTime;
    int ioTime;
    burst *burstToAdd;
    burst *burstList = NULL;
    char buffer[50];
    char *tempLine;
    char *token;
    int i;
    
    thread *newThread;
    newThread = malloc(sizeof(thread));
    newThread->tid = threadID;
    newThread->pid = processID;
    newThread->arriveTime = arriveTime;
    newThread->numOfBursts = numBursts;
    newThread->state = "new";
    
    //get the line of the first burst
    //parse the line and assigin it to the appropriate variables
    for (i=0;i<numBursts;i++){
    tempLine = getLine(fp, buffer);
        token = strtok(tempLine, " ");
        burstNum = atol(token);
        token = strtok(NULL, " ");
		if (token == NULL){
			cpuTime = 0;
			ioTime = 0;
		}else {
        	cpuTime = atol(token);
        	token = strtok(NULL, " ");
			if (token == NULL)
				ioTime = 0;
			else
				ioTime = atol(token);
		}
        //create the burst
        burstToAdd = createBurst(burstNum, cpuTime, ioTime);
        //ands the new burst to the list of burst

        burstList = addBurst(burstToAdd, burstList);
    }
    
    newThread->totalCPU = addCPUTime(burstList);
    newThread->totalIO = addIOTime(burstList);
    printf("cpu time: %d, io time: %d\n",newThread->totalCPU,newThread->totalIO);
    newThread->firstBurst = burstList;
    newThread->next = NULL;
	burstList = NULL;    
    return newThread;
}

thread *addThread(thread *threadToAdd, thread *threadList){
    if (threadList == NULL){
        threadList = threadToAdd;
    }else {
        //find the last item in the list        
        int check = 0;
        thread *currentThread  = threadList;
        while (check==0){
            if (currentThread->next == NULL){
                check = 1;
                currentThread->next = threadToAdd;
            } else {
                currentThread = currentThread->next;
			}
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
    burst *currentBurst = burstList;
    if (burstList == NULL){
        burstList = burstToAdd;
    }else {
        //find the last item in the list
        int check = 0;
        while (check==0){
            if (currentBurst->next == NULL){
                check = 1;
                currentBurst->next = burstToAdd;
            } else {
                currentBurst = currentBurst->next;
        	}
		}
    }
    return burstList;
}

int addCPUTime(burst *burstList){
    int sum;
    burst *currentBurst = burstList;
    
    while (currentBurst->next != NULL) {
        sum = sum + currentBurst->cpuTime;
        currentBurst = currentBurst->next;
    }
    //get the last one
    sum = sum + currentBurst->cpuTime;
    
    return sum;
}

int addIOTime(burst *burstList){
    int sum;
    burst *currentBurst = burstList;
    
    while (currentBurst->next != NULL) {
        sum = sum + currentBurst->ioTime;
        printf("io burst: %d\n",currentBurst->ioTime);
        currentBurst = currentBurst->next;
    }
    //get the last one
    sum = sum + currentBurst->ioTime;
    
    return sum;
    
    
}
