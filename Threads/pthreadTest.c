#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void * threadExecute(void * arg);



int main(int argc, char **argv){

	int number = 4;
	int check;

	//launching a thread
	//thread ID
	pthread_t threadID;
	//attributes
	pthread_attr_t attr;
	pthread_attr_init(&attr);


	check = pthread_create(&threadID,&attr, threadExecute,&number);
	if (check != 0){
		printf("Thread was not created\n");
	}

	//wait for thread to finish
	pthread_join(threadID, NULL);

}

void * threadExecute(void * arg){

	int num = * (int*) arg;
	printf("In thread: %d\n",num);
	wait(1);
	printf("done\n");
	pthread_exit(0);
}




