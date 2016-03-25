#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>


int numPhils;
int numEats;
pthread_t *phil;
pthread_mutex_t *chopstick;
int *chopstickAvail;
int *eatsLeft;
int *state;

int main(int argc, char *argv[]){
    
    if (argc != 3){
        printf("Not enough information\n");
        exit(0);
    }
    
    numPhils = atol(argv[1]); //number of phils
    numEats = atol(argv[2]); //number of times each phil must eat
    
    phil = malloc(numPhils * sizeof(pthread_t)); //thread for each philosopher
    chopstick = malloc(numPhils * sizeof(pthread_mutex_t));//mutex for each chopstick
    chopstickAvail = malloc(numPhils *sizeof(int));
    
    int i = 0;
    for (i=0,i<numPhils;i++){
        chopstickAvail[i] = 0;
        
    }
    i = 0;
    for (i=0;i<numPhils;i++){
        pthread_mutex_init(&chopstick[i], NULL);
    }
    
    i = 0;
    for (i=0;i<numPhils;i++){
        pthread_create(&phil[i], NULL, philosopher, (void *)i);
        printf("Philosopher %d: thinking\n",i);
    }
    return 0;
}

void *philosopher (void *threadID){
    
    int rightChopstick, leftChopstick;
    int id = (int)threadID;
    int i = 0;
    while (i=0;i<numEats[i];i++){ //eat multiple times -- change to for loop
        //get th ids of the chopstick the phil would use
        rightChopstick = id;
        if (id+1 == numPhils)
            leftChopstick = 0;
        else 
            leftChopstick = id + 1;
        
        printf("Philosopher %d: hungry\n",id);
        while(chopstickAvail[rightChopstick] == 1 || chopstickAvail[leftChopstick] == 1){

        }
        pthread_mutex_lock(&chopstick[rightChopstick]);
        chopstickAvail[rightChopstick] = 1;
        pthread_mutex_lock(&chopstick[leftChopstick]);            
        chopstickAvail[leftChopstick] = 1;
        printf("Philosopher %d: eating\n",id);
        pthread_mutex_unlock(&chopstick[rightChopstick]);
        chopstickAvail[rightChopstick] = 0;
        pthread_mutex_unlock(&chopstick[leftChopstick]);
        chopstickAvail[leftChopstick] = 0;
        printf("Philosopher %d: thinking\n",id);
    }
    pthread_exit(NULL);
}