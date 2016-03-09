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
process *createNewProcess(FILE *fp, int id, int threadNum);
process *addProcess(process *processToAdd, process *processList);
thread *createNewThread(FILE *fp, int threadID, int cpuTime, int ioTime);
thread *addThread(thread *threadToAdd, thread *threadList);
burst *createBurst(int burstNum, int cpuTime, int ioTime);
burst *addBurst(burst *burstToAdd, burst *burstList);

int main (int argc, char *argv){

    clock_t start, end, total;
	start = clock();
    int numProcesses;
    int threadSwitchTime;
    int processSwitchTime;
    char buffer [500];
    process *processList;
    char fileName[100] = "inputFile.txt";
    int i=0;
    
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
    char *tempLine;
    
    
    
    //make a list of processes
    for (i=0;i<numProcesses;i++){
		process *processToAdd;
        tempLine = getLine(fp, buffer);
        token = strtok(tempLine, " ");
        int processNum = atol(token);
        token = strtok(NULL, " ");
        int threadNum = atol(token);
        
        processToAdd = createNewProcess(fp,processNum,threadNum);
        processList = addProcess(processToAdd, processList);
       
    }
    end = clock();
	total = (float)(end - start)*1000;
    printf("CPU time: %f\n",total);
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

process *createNewProcess(FILE *fp, int id, int threadNum){
    
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
    //parse the line and assigin it to the appropriate variables
    for (i=0;i<threadNum;i++){
        char buffer[100];
        tempLine = getLine(fp, buffer);
        printf("tempLine: %s\n",tempLine);
        token = strtok(tempLine, " ");
        threadID = atol(token);
        token = strtok(NULL, " ");
        arriveTime = atol(token);
        token = strtok(NULL, " ");
        numBursts = atol(token);
        threadList = NULL;
        //creates a new thread and give the address of the new thread to threadToAdd
        threadToAdd = createNewThread(fp, threadID, arriveTime, numBursts);
		printf("one thread made\n");
        //ands the new thread to the list of threads
        threadList = addThread(threadToAdd, threadList);
        printf("thread added\n");
        printf("the current line: %s\n",tempLine);
    }
    newProcess->firstThread = threadList;
    newProcess->next = NULL;
    return newProcess;
}

process *addProcess(process *processToAdd, process *processList){
    if (processList == NULL)
        processList = processToAdd;
    else {
        //find the last item in the list
        
        int check = 0;
        while (check==0){
            process *currentProcess = processList;
            if (currentProcess->next == NULL){
                check = 1;
                currentProcess->next = processToAdd;
                
            } else 
                currentProcess = currentProcess->next;    
        }
    }
    return processList;
    
    
}

thread *createNewThread(FILE *fp, int threadID, int arriveTime, int numBursts){
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
    newThread->arriveTime = arriveTime;
    newThread->numOfBursts = numBursts;
    
	printf("number of bursts: %d\n",numBursts);

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
        //tempLine = getLine(fp,buffer);
    }
    
    newThread->firstBurst = burstList;
    newThread->next = NULL;
	burstList = NULL;
    printf("current Line: %s\n",tempLine);
    
    return newThread;
}

thread *addThread(thread *threadToAdd, thread *threadList){
    printf("in addThread: %p\n",threadList);
    if (threadList == NULL){
    threadList = threadToAdd;
    }else {
        //find the last item in the list
        printf("here1\n");
        
        int check = 0;
        thread *currentThread  = threadList;
        printf("here2\n");
        while (check==0){
        printf("here3\n");
            if (currentThread->next == NULL){
                check = 1;
                currentThread->next = threadToAdd;
                printf("adding thread\n");
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

	printf("%d %d %d\n",newBurst->burstNum, newBurst->cpuTime, newBurst->ioTime);
    return newBurst;
}

burst *addBurst(burst *burstToAdd, burst *burstList){
    burst *currentBurst = burstList;
    printf("%p, %p\n",burstList, currentBurst);
    if (burstList == NULL){
		printf("Was first burst\n");
        burstList = burstToAdd;
    }else {
        //find the last item in the list
        int check = 0;
        while (check==0){
            if (currentBurst->next == NULL){
                check = 1;
                currentBurst->next = burstToAdd;
                printf("adding burst\n");
            } else {
                currentBurst = currentBurst->next;
				printf("in between\n");
        	}
		}
    }
    return burstList;
}
