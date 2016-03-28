
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#define THINKING (0)
#define HUNGRY (1)
#define EATING (2)
#define AVAIL (0)
#define UNAVAIL (1)

typedef struct Philosopher{
	int id;
	int  state;
	pthread_t tid;
}phil;

typedef struct Chopstick{
	int id;
	int state;
	pthread_mutex_t mutex;
}cs;

int numPhils;
int numEats;
phil **philList;
cs **csList;

phil *addPhil(int id);
cs *addCS(int id);
void *philosopher(void *threadID);

int main(int argc, char *argv[]){
    
    if (argc != 3){
        printf("Not enough information.\n Please enter the number of philosophers and how many times they will eat.\n");
        exit(0);
    }
    
    numPhils = atol(argv[1]); //number of phils
    numEats = atol(argv[2]); //number of times each phil must eat
	csList = malloc(sizeof(cs)*numPhils);
	philList = malloc(sizeof(phil)*numPhils);

    int i = 0;
    for (i=0;i<numPhils;i++) {
        philList[i] = addPhil(i);
    	csList[i] = addCS(i);
	}
    i = 0;
    for (i=0;i<numPhils;i++){
        pthread_mutex_init(&csList[i]->mutex, NULL);
    }
    
    int j = 0;
    for (j=0;j<numPhils;j++){
        pthread_create(&philList[j]->tid, NULL, philosopher, &philList[j]->id);
    }

     i = 0;
     for (i=0;i<numPhils;i++){
        pthread_join(philList[i]->tid,NULL);
    }
    return 0;
}

phil *addPhil(int id){
	phil *newPhil;
	newPhil = malloc(sizeof(phil));
	newPhil->id = id;
	newPhil->state = THINKING;
	return newPhil;
}

cs * addCS(int id){
	cs *newCS;
	newCS = malloc(sizeof(cs));
	newCS->id = id;
	newCS->state = AVAIL;
	return newCS;
}

void *philosopher (void *threadID){
    
    int rightCS, leftCS;
    int id = *(int*)threadID;
    int i = 0;
    
    printf("Philosopher %d is thinking.\n",id);
	sleep((rand() % 5) +1);
    for (i=0;i<numEats;i++){ //eat multiple times -- change to for loop
        //get th ids of the chopstick the phil would use
        rightCS = id;
        if (id+1 == numPhils)
            leftCS = 0;
        else 
            leftCS = id + 1;
        
        printf("Philosopher %d is hungry.\n",id);
        while(csList[rightCS]->state != AVAIL && csList[leftCS]->state != AVAIL){
        }
        
        pthread_mutex_lock(&csList[rightCS]->mutex);
        csList[rightCS]->state = UNAVAIL;
        pthread_mutex_lock(&csList[leftCS]->mutex);
        csList[leftCS]->state = UNAVAIL;
		printf("Philosopher %d is eating.\n",id);
        sleep((rand()%5)+1);

        pthread_mutex_unlock(&csList[rightCS]->mutex);
        csList[rightCS]->state = AVAIL;
        pthread_mutex_unlock(&csList[leftCS]->mutex);
        csList[leftCS]->state = AVAIL;
        printf("Philosopher %d is thinking.\n",id);
        sleep((rand()%5)+1);
    }
    pthread_exit(NULL);
}
