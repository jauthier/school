/* getThreads.c
 * contains all the functions involved in parsing the input file and 
 * creating the list of threads
 */

#include "getThreads.h"

int getNumThreads(thread* threadList){
    int num = 0;
    while (threadList != NULL){
        num++;
		threadList = threadList->next;
    }
    return num;
}

thread *sortList(thread *threadList, int numThreads){
    
    if (threadList == NULL) //no threads
        return threadList;
    if (threadList->next ==NULL) //only one thread
        return threadList;
    
    
    thread* sortedList = threadList; //set sortedList to the first thread of threadList
    threadList = threadList->next; //set threadList to its second thread
    sortedList->next = NULL; //it is the only thread in the list
    int count = 1; //starts at 1 because there is on thread in the sorted list
    printf("in sort\n");
    while((numThreads-1) != 0){ //while there is another thread to get from the original list
        thread *previous = NULL;
        int i = 0;
        thread *currentThread = sortedList;//start at the beginning of the sorted list
        for(i=0;i<count;i++){ //iterate through sorted List
            if ((threadList->arriveTime < currentThread->arriveTime)&& i == 0){ //arrived sooner then the first one
                thread* hold = sortedList; // hold the previous first
                sortedList = threadList; //assign new first thread
                threadList = threadList->next; //get the next thread to check
                sortedList->next = hold; //add the previous first after
                break;
            }else if (threadList->arriveTime == currentThread->arriveTime && 1 == 0){
                thread *hold = currentThread->next; //hold the thread that will be after the thread being inserted
                currentThread->next = threadList;
                threadList = threadList->next;
                currentThread->next->next = hold;
                break;
            }else if ((threadList->arriveTime < currentThread->arriveTime)&& (i <= count-1)){ //in between two
                previous->next = threadList;
                threadList = threadList->next;
                previous->next->next = currentThread;
                break;
            }else if ((threadList->arriveTime == currentThread->arriveTime)&& (i <= count-1)){ //in between two

                thread *hold = currentThread->next; //hold the thread that will be after the thread being inserted
                currentThread->next = threadList;
                threadList = threadList->next;
                currentThread->next->next = hold;
                break;
            } else if ((threadList->arriveTime >= currentThread->arriveTime)&& (i == count-1)){ //at the very end
                currentThread->next = threadList;
                threadList = threadList->next;
                currentThread->next->next = NULL;
                break;
            }else {
                previous = currentThread;
                currentThread = currentThread->next;
            }
        }
        numThreads--;
        count ++;
    }
    
    return sortedList;
}

void printList(thread *list){
    
    while(list != NULL){
        printf("arrive: %d, pid: %d, tid: %d\n",list->arriveTime,list->pid,list->tid);
        list = list->next;
    }
}

thread *loadThreads(){

    int numProcesses;
    char buffer [50];
    thread *threadList = NULL;
    int i=0;
    char *tempLine = malloc(sizeof(char)*20);
    
    /* Get the first line of the input file, this will tell how many processes there are,
    the time it takes to switch beteen threads of the same process and the time it takes
    to switch between threads of different processes */
    
	//char *firstline = getLine(fp, buffer);
    char *firstLine = malloc(sizeof(char)*20);
    fgets(buffer, 50, stdin);
    strcpy(firstLine, buffer);
    char *token = strtok(firstLine," ");
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
        fgets(buffer,50,stdin);
        strcpy(tempLine,buffer);
        //parse the line
        token = strtok(tempLine, " ");
        processNum = atol(token);
        token = strtok(NULL, " ");
        threadNum = atol(token);
        
        //get the right amount of threads
        for (j=0;j<threadNum;j++){
            char threadBuffer[50];
            fgets(threadBuffer, 50, stdin);
            strcpy(tempLine,threadBuffer);

            token = strtok(tempLine, " ");
            threadID = atol(token);
            token = strtok(NULL, " ");
            arriveTime = atol(token);
            token = strtok(NULL, " ");
            numBursts = atol(token);
            //creates a new thread and give the address of the new thread to threadToAdd
            threadToAdd = createNewThread(processNum, threadID, arriveTime, numBursts);
            //ands the new thread to the list of threads
            threadList = addThread(threadToAdd, threadList);
        }
    }
    return threadList;
}

thread *createNewThread(int processID, int threadID, int arriveTime, int numBursts){
    int burstNum;
    int cpuTime;
    int ioTime;
    burst *burstToAdd;
    burst *burstList = NULL;
    char buffer[50];
    char *tempLine = malloc(sizeof(char)*20);
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
        fgets(buffer, 50, stdin);
        strcpy(tempLine, buffer);
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
    int sum = 0;
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
    int sum = 0;
    burst *currentBurst = burstList;
    while (currentBurst->next != NULL) {
        sum = sum + currentBurst->ioTime;
        currentBurst = currentBurst->next;
    }
    return sum;
}
