#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void fcfsRun();
void rrRun();
void checkArrival();
void checkRunningFCFS();
void checkRunningRR();
void checkWait();
thread *getLast (thread *threadList);

int main (int argc, char *argv[]){
    
    char type = 't';
	quantum = 50;
    char fileName[100];
    
    if (argc < 3){
        printf("Invaid Input.\n");
        exit(0);
    }
    int i = 1;
    for (i=1;i<argc;i++){ //skip the first one
        if (strcmp(argv[i],"-d")){
            detailed = true;
        }else if (strcmp(argv[i],"-v")){
            verbose = true;
        }else if (strcmp(argv[i],"-r")){
            type = 'r';
            quantum = atol(argv[i+1]);
        }else if (strcmp(argv[i],"<")){
            strcpy(fileName, argv[i+1]);
        }
    }
    if (type == 'f')
        printf("FCFS Scheduling\n");
    else 
        printf("RR Scheduling\n");
    
    FILE* fp = fopen(fileName,"r");
    thread *threadList;
    threadList = loadThreads(fp);
    fclose(fp);
    
    // initialize the CPU
	CPU = malloc(sizeof(processor));
    CPU->switching = false;
    CPU->timeLeft = 0;
    CPU->running = NULL;
        
    printList(threadList);
    int numThreads = getNumThreads(threadList);
    threadList = sortList(threadList, numThreads);
    printList(threadList);
    notArrivedYet = threadList;

    if (type == 'f')
        fcfsRun();
    else if (type == 'r')
        rrRun();
    
    printf("Total time required is %d time units\n",counter);
    
    int sum = 0;
    int count = 0;
    thread *temp = terminated;
    while (temp != NULL){
        sum = sum + (temp->endTime - temp->arriveTime);
        count++;
    }
    float avTurn = sum/count;
    printf("The average turnaround time was %f time units.\n",avTurn);
    
    //print out thread details  --- turnaround time = end time - arrival time
    
    return 0;
}

void fcfsRun(){

    while(readyQueue != NULL || waitQueue != NULL || notArrivedYet != NULL){
		printf("\nCOUNT: %d\n",counter);
        if (notArrivedYet != NULL)
		    checkArrival();
//    char c = getchar();
		checkRunningFCFS();

		if (CPU->switching == false && CPU->running != NULL){
        	printList(CPU->running);
        	printf("%d\n",CPU->running->firstBurst->cpuTime);
        }else
			printf("switching\n");

        checkWait();
        printList(waitQueue);
        counter ++;
    }
}

void rrRun(){

    while(readyQueue != NULL || waitQueue != NULL || notArrivedYet != NULL){
		printf("\nCOUNT: %d\n",counter);
        if (notArrivedYet != NULL)
		    checkArrival();
		checkRunningRR();

		if (CPU->switching == false && CPU->running != NULL){
        	printList(CPU->running);
        	printf("%d\n",CPU->running->firstBurst->cpuTime);
        }else
			printf("switching\n");

        checkWait();
        printList(waitQueue);
        counter ++;
    }
}

void checkArrival(){
    thread *checkThread = notArrivedYet;
    while ((checkThread != NULL) && (checkThread->arriveTime == counter)){
        thread *lastThread = getLast(readyQueue); //get the last thread in the ready queue
        if (checkThread->next == NULL){ // if this is the last thread in the not srrived queue
            if (lastThread == NULL){
                checkThread->state = "ready";
                readyQueue = checkThread; //this is the first in the ready queue
            } else {
                lastThread->next = checkThread; //add thready to the end of the ready queue
				notArrivedYet = NULL;
                checkThread = NULL;
			}
        } else {
            thread *hold = checkThread->next; // holds the following threads
                
            if (lastThread == NULL){
                checkThread->next = NULL;
                checkThread->state = "ready";
				readyQueue = checkThread; //this is the first in the ready queue
			}
            else{
                checkThread->next = NULL;
                checkThread->state = "ready";
                lastThread->next = checkThread; //add thready to the end of the ready queue
            }
            checkThread = hold; //move the second thread to first
            notArrivedYet = hold;
        }
    }
}

void checkRunningFCFS(){
    
    if (CPU->running != NULL){
        int runTimeLeft = CPU->running->firstBurst->cpuTime - (counter - CPU->running->firstBurst->runStartTime);
        if (runTimeLeft == 0){ // the process is done
            //take it out and add it to the waiting queue
            //set CPU to switching = true and timeLeft to the proper number, need to know about the next thread coming in
            CPU->switching = true;
            if (CPU->running->pid == readyQueue->pid)
                CPU->timeLeft = threadSwitchTime;
            else
                CPU->timeLeft = processSwitchTime;
            
            thread *last;
            
            if (CPU->running->firstBurst->ioTime == 0){ //if that was the last burst
                CPU->running->state = "terminated";
                CPU->running->firstBurst = NULL;
                last  = getLast(terminated);
                if (last == NULL)
                    terminated = CPU->running;
                else 
                    last-> next = CPU->running;
            }else {
                printf("here\n");
                CPU->running->state = "blocking";
                last = getLast(waitQueue);
                if (last == NULL)
                    waitQueue = CPU->running;
                else
                    last->next = CPU->running;
            }
            CPU->running = NULL;
        }
    } else if ((CPU->running == NULL)&&(CPU->switching == true)){
        if (CPU->timeLeft <= 2){ //on its last one
            CPU->switching = false;
        } else {
            CPU->timeLeft--;
        }
    } else if ((CPU->running == NULL)&&(CPU->switching == false)){ //nothing being processed and there is no switching in progress
        readyQueue->firstBurst->runStartTime = counter; //when the burst started
        thread *hold = readyQueue->next; //hold the next thread in the ready queue
        CPU->running = readyQueue; //send the first thread to running
        CPU->running->next = NULL;
        CPU->running->state = "running";
        readyQueue = hold; //move the next thread to the start of the ready queue
    }
}

void checkRunningRR(){
    
    if (CPU->running != NULL){
        int runTimeLeft = CPU->running->firstBurst->cpuTime - (counter - CPU->running->firstBurst->runStartTime);
        if (runTimeLeft == 0 || quantumLeft == 0){ // the process is done or its time is up
            //take it out and add it to the waiting queue
            //set CPU to switching = true and timeLeft to the proper number, need to know about the next thread coming in
            CPU->switching = true; //set the CPU to switching
            //set the amount of time required to switch
            if (readyQueue != NULL){
                if (CPU->running->pid == readyQueue->pid)
                    CPU->timeLeft = threadSwitchTime;
                else
                    CPU->timeLeft = processSwitchTime;
            }
            thread *last;

            if (runTimeLeft == 0){ //the thread have finished
                if (CPU->running->firstBurst->ioTime == 0){ //if this was the last burst
                    printf("At time %d: change\n",counter);
                    CPU->running->state = "terminated";
                    CPU->running->firstBurst = NULL;
                    last  = getLast(terminated);
                    //send to the list of terminated threads
                    if (last == NULL)
                        terminated = CPU->running;
                    else 
                        last-> next = CPU->running;
                }else { //if this wasnt the last burst
                    printf("At time %d: Thread %d of process %d moves from %s to blocking.\n", counter, CPU->running->tid, CPU->running->pid, CPU->running->state);
                    CPU->running->state = "blocking"; //change state of thread
                    //send to the waiting queue
                    last = getLast(waitQueue);
                    if (last == NULL)
                        waitQueue = CPU->running;
                    else
                        last->next = CPU->running;
                }
            }else { //the pthread ran out of time
                CPU->running->firstBurst->cpuTime = runTimeLeft; //so it knows how much time is left next time it runs
                last = getLast(readyQueue);
                if (last == NULL)
                    readyQueue = CPU->running;
                else 
                    last->next = CPU->running;
            }
            
            
            CPU->running = NULL; //say there is no process here
        }else 
            quantumLeft--;
    } else if ((CPU->running == NULL)&&(CPU->switching == true)){
        if (CPU->timeLeft <= 2){ //on its last one
            CPU->switching = false;
        } else {
            CPU->timeLeft--;
        }
    } else if ((CPU->running == NULL)&&(CPU->switching == false)&&(readyQueue != NULL)){ //nothing being processed and there is no switching in progress
printf("here1\n");
        readyQueue->firstBurst->runStartTime = counter; //when the burst started
        thread *hold = readyQueue->next; //hold the next thread in the ready queue
        CPU->running = readyQueue; //send the first thread to running
        CPU->running->next = NULL;
        CPU->running->state = "running";
        quantumLeft = quantum; //set time limit
        readyQueue = hold; //move the next thread to the start of the ready queue
    }
}

void checkWait(){
    thread *check = waitQueue;
    thread *previous = NULL;
    while (check != NULL){
        //look at io burst time of each
        if (check->firstBurst->ioTime == 0){ //finished io
            thread *last = getLast(readyQueue);
            thread *start = NULL;
            if (previous == NULL){
                thread *hold = check->next;
                check->next = NULL;
                check->firstBurst = check->firstBurst->next; //get rid of the finished bursts
                if (last != NULL)
                    last->next = check; //add the finished thread to the 
                else 
                    readyQueue = check;
                if (hold == NULL){
					check = NULL;
				    waitQueue = NULL;
                }else {
                    check = hold;
                    waitQueue = hold;
                }
            }else {
                thread *hold = check->next;
                check->next = NULL;
                check->firstBurst = check->firstBurst->next; //get rid of the finished bursts
                if (last != NULL)
                    last->next = check; //add the finished thread to the 
                else 
                    readyQueue = check;            
                if (hold == NULL){
					check = NULL;
				    waitQueue = NULL;
                }else {
                    check = hold;
                    waitQueue = hold;
                }
            }
        }else {
            check->firstBurst->ioTime--;
            previous = check;
            check = check->next;
        }
    }
}

thread *getLast (thread *threadList){

    thread *tempList;
	if ( threadList == NULL)
		tempList = NULL;
	else {
    	tempList = threadList;
    	while(tempList->next != NULL){
        	tempList = tempList->next;
    	}
    }
    return tempList;
}

