
#include "history.h"

int writeHistory(char *toWrite){

    char *fileName = malloc(sizeof(char)*100);
    strcpy(fileName, rootDir);
    strcat(fileName,"/");
    strcat(fileName,".CIS3110_history");
    FILE* fp;
    fp = fopen(fileName,"a");
    if (fp == NULL){
        printf("%s: No such file or direcotry\n",fileName);
        return 0;
    }
    fprintf(fp,"%s",toWrite);

    fclose(fp);
    free(fileName);
	return 0;
}

void readHistory(){

    char buffer[100];
    char *fileName = malloc(sizeof(char)*100);
    strcpy(fileName, rootDir);
    strcat(fileName,"/");
    strcat(fileName,".CIS3110_history");
    FILE* fp;
    fp = fopen(fileName,"r");
    if (fp == NULL)
        printf("%s: No such file or direcotry\n",fileName);

    int count = 1;
    while (fgets(buffer,100,fp)!= NULL){
	    printf(" %d  %s\n",count,buffer);
	    count++;
    }
    fclose(fp);
    free(fileName);
}

void clearHistory(){

    char *fileName = malloc(sizeof(char)*100);
    strcpy(fileName, rootDir);
    strcat(fileName,"/");
    strcat(fileName,".CIS3110_history");
    FILE* fp;
    fp = fopen(fileName,"w");
    if (fp == NULL)
        printf("%s: No such file or direcotry\n",fileName);

    fprintf(fp," ");
    
    fclose(fp);
    free(fileName);
}

int readnEntries(char *arg){
    
    char *endPtr;
    
    if (strtol(arg, &endPtr,10) == 0){
        printf("%s: command not found\n",arg);
        return 0;
    } else{
        
		char buffer[100];
        int numEntries = strtol(arg, &endPtr,10);
        char *fileName = malloc(sizeof(char)*100);
        strcpy(fileName, rootDir);
        strcat(fileName,"/");
        strcat(fileName,".CIS3110_history");
        FILE* fp;
        fp = fopen(fileName,"r");
        if (fp == NULL)
            printf("%s: No such file or direcotry\n",fileName);
        
        int count = 1;
        while ((fgets(buffer,100,fp)!= NULL) && (count <= numEntries)){
            printf(" %d  %s\n",count,buffer);
            count++;
        }
        fclose(fp);
        free(fileName);
    }
    return 0;
}
