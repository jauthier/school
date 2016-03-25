#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typeded struct process{
    char id;
    int size;
    process next;
}process;


int main(int argc, char* argv[]){
    
    if (argc != 2){
        printf("You must enter an input file\n");
        exit(0);
    }
    
    char buffer[10];
    
    char* fileName;
    strcpy(fileName, argv[1]);
    
    FILE *fp = fopen(fileName, "r");
    
    getLine(fp);
    
    
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
        //pineapple
        
        token = strtok(line, " ");
        strcpy(buffer, token);
        id = buffer[0];
        token = strtok(NULL, " ");
        size = atol(token);
        process processToAdd = makeProcess(id, size);
        list = addProcess(processToAdd<);
        
        line = getLine(fp, line);
    }
    
}

process *makeProcess(char id, int size){
    process *newProcess;
    newProcess = malloc(sizeof(process));
    newProcess->id = id;
    newProcess->size = size;
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
