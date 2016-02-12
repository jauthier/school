
#include "redirection.h"

/*returns 1 if ther is a <*/
int checkInRedir(char **args, int numArgs){

    int i = 0;
    for (i=0;i<numArgs;i++){
        if (strcmp(args[i], "<")==0){
            return 1;
        }
    }
    return 0;
}

/*returns 1 if ther is a >*/
int checkOutRedir(char **args, int numArgs){

    int i = 0;
    for (i=0;i<numArgs;i++){
        if (strcmp(args[i], ">")==0){
            return 1;
        }
    }
    return 0;
}

int outRedir(char **args, int numArgs){

    int i = 0;
    int j = 0;
    char outFile[20];
    char *newArgs[10];
    FILE *fp;

    //determines where in the array the > is
    for (i=0;i<numArgs;i++){
        if (strcmp(args[i], ">")==0)
            break;
    }
    //gets the name of the file
    if (args[i+1] != NULL){
        strcpy(outFile, args[i+1]);
    } else {
        printf(": syntax error\n");
        return 0;
    }
    //make a new array with the commands that willbe sent to execProcess
    for (j=0;j<i;j++){
        newArgs[j] = args[j];
    }
    newArgs[j] = NULL;

    //open file stream
    fp = freopen(outFile,"w",stdout);
    if (fp == NULL){
        printf("%s: No such file or directory\n",outFile);
        return 0;
    }
    //send to get executed
    execProcess(newArgs, j);
    //close file
    fclose(fp);

    return 0;
}

int inRedir(char ** args, int numArgs){
    //command < filename
    //read file and give it as input to the command
    
    int i = 0;
    int j = 0;
    char inFile[20];
    char *newArgs[10];
    FILE *fp;
    char buffer[200];
    char *token;
    
    /* determines where in the array the < is */
    for (i=0;i<numArgs;i++){
        if (strcmp(args[i], "<")==0)
            break;
    }// i give the position of the <

    //gets the name of the file
    if (args[i+1] != NULL){
        strcpy(inFile, args[i+1]);
    } else {
        printf(": syntax error\n");
        return 0;
    }
    
    //make a new array with the commands that willbe sent to execProcess
    for (j=0;j<i;j++){
        newArgs[j] = args[j];
    }
    newArgs[j] = NULL;
    
    //open file stream
    fp = fopen(inFile,"r");
    if (fp == NULL){
        printf("%s: No such file or directory\n",inFile);
        return 0;
    }
    
    fgets(buffer, 200, fp);
    
     //add the read in string to the end of the char* array
        token = strtok(buffer, " \n");
        int n = j;
        while(token!=NULL){
            newArgs[n] = token;
            token = strtok(NULL, " \n");
            n++;
        }
        newArgs[n] = NULL;
    
    
    //send to get executed
    execProcess(newArgs, j);
    //close file
    fclose(fp);
    
    return 0;
}
