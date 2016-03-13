#ifndef getThreads_h
#define getThreads_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct burst {
    int burstNum;
    int runStartTime;
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

enum boolean{
    false,
    true
};

typedef struct processor {
    enum boolean switching;
    int timeLeft;
    thread *running;
} processor;

int counter;
int threadSwitchTime;
int processSwitchTime;
int quantum;
int quantumLeft; //how much time the current running process has left
thread *notArrivedYet;
thread *readyQueue;
thread *waitQueue;
thread *terminated;
processor *CPU;
enum boolean detailed;
enum boolean verbose;

int getNumThreads(thread* threadList);
thread *sortList(thread *threadList, int numThreads);
void printList(thread *list);
thread *loadThreads(FILE* fp);
char *getLine(FILE *fp, char *line);
thread *createNewThread(FILE *fp, int processID, int threadID, int cpuTime, int ioTime);
thread *addThread(thread *threadToAdd, thread *threadList);
burst *createBurst(int burstNum, int cpuTime, int ioTime);
burst *addBurst(burst *burstToAdd, burst *burstList);
int addCPUTime(burst *burstList);
int addIOTime(burst *burstList);


#endif
