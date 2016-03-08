#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

struct thread {
	int cpuTime;
	int ioTime;
	struct thread *next;
};

struct Process {
	int numThreads;
	struct thread firstThread;

} process;


int main(int argc, char** argv){


	return 0;
}

//void parse(){

//}
