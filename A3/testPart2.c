#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOTALMEM (128)

typeded struct process{
    char id;
    int size;
    int numSwaps;
    int startLocation;
    int endLocation;
    process next;
}process;

char memory[TOTALMEM];
process *pList;
process *finished;
process *waiting;
process *loaded;

int main(int argc, char* argv[]){
    
    if (argc != 2){
        printf("You must enter an input file\n");
        exit(0);
    }
    
    char buffer[10];
    
    char* fileName;
    strcpy(fileName, argv[1]);
    
    FILE *fp = fopen(fileName, "r");
    *pList = parseFile(fp);
    fclose(fp);
    initMem();
    firstFit();
    
    //
    
    return 0;
}

void initMem(){
    int i = 0;
    for (i=0;i<TOTALMEM;i++){
        memory[i] = '!'; //the space is empty
    }
}

void firstFit(){
        
    int nextEmpty;
    int enoughSpace;
    
    printf("First Fit.\n");
    waiting = pList; //all start off waiting
    
    while (waiting != NULL){
        nextEmpty = getNextEmpty();
        if (nextEmpty == 129){
            swap();
        }else {
            enoughSpace = checkSpace(nextEmpty, waiting->size);
            if (enoughSpace == 0){ //not enough
                swap();
            }else { //enough
                loadToMemory(loaded, nextEmpty); //load to memory
            }
        }
    }
}

int getNextEmpty(){
    int i = 0;
    for (i=0;i<TOTALMEM;i++){
        if (memory[i] == '!');
            return i;
        else 
            continue;
    }
    return 129; //check for this then will know all spots are taken
}

int checkSpace(int spaceStart, int sizeNeeded) {
    int i;
    int count = 0;
    for (i=spaceStart;i<TOTALMEM;i++){
        if (memory[i] == '!')
            count++;
    }
    if (count<=sizeNeeded)
        return 1;
    
    return 0;
}

void loadToMemory (int startLoc){
    int i;
    process *hold = waiting->next; //hold the next one waiting
    process *last;
    waiting->next = NULL;
    waiting->startLocation = startLoc;
    waiting->endLocation = startLoc + waiting->size - 1;
    for (i=startLoc;i<=waiting->endLocation;i++){
        memory[i] = waiting->id; //set the spots in memory so it knows this process it there
    }
    last = getLast(loaded); //see if there are other processes loaded
    if (last == NULL)
        loaded = waiting;
    else 
        last->next = waiting;
    waiting = hold;
}

void swap(){
    //swap out the first one in the loaded queue
    //take it out then check again
    int i;
    int nextEmpty;
    int enoughSpace;
    
    do {
        process *hold = loaded->next; //hold the rest of the
        process *last = getLast(waiting);
        for (i=loaded->startLocation;i<=loaded->endLocation;i++){
            memory[i] = '!';
        }
        loaded->startLocation =129;
        loaded->endLocation = 129;
        loaded->next = NULL;
        if (last == NULL)
            waiting = loaded;
        else
            last->next = loaded;
        loaded = hold;
        nextEmpty = getNextEmpty();
        if (nextEmpty != 129)
            enoughSpace = checkSpace(nextEmpty,waiting->size);
        else
            enoughSpace = 0
    }while (enoughSpace == 0);
    
    loadToMemory(nextEmpty);
}




process *getLast (process *list){

    process *tempList;
	if ( list == NULL)
		tempList = NULL;
	else {
    	tempList = list;
    	while(tempList->next != NULL){
        	tempList = tempList->next;
    	}
    }
    return tempList;
}

process *parseFile(FILE *fp){
    
    char *token;
    char *line = malloc(sizeof(char)*15);
    process *pList = NULL;
    line = getline(fp, line);
    
    while(line != NULL){
        
        char buffer[2];
        char id;
        int size;
        
        token = strtok(line, " ");
        strcpy(buffer, token);
        id = buffer[0];
        token = strtok(NULL, " ");
        size = atol(token);
        process processToAdd = makeProcess(id, size);
        pList = addProcess(processToAdd, pList);
        
        line = getLine(fp, line);
    }
    return pList;
}

process *makeProcess(char id, int size){
    process *newProcess;
    newProcess = malloc(sizeof(process));
    newProcess->id = id;
    newProcess->size = size;
    newProcess->numSwaps = 0;
    newProcess->startLocation = 129; //not in memory
    newProcess->endLocation = 129; //not in memory
    newProcess->next = NULL;
    return newProcess;
}

process *addProcess(process *toAdd, process *list){
    process *temp = list;
    if (temp == NULL){
        list = toAdd;
    }else {
        while (temp != NULL){
            temp = temp->next;
        }
        temp->next = toAdd;
    }
    return list;
}

char* getLine(FILE* fp, char *line){
    
    if (fp == NULL) {
        printf("Unable to open file.\n");
        exit(0);
    }
    
    int maxLen = 15;
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
