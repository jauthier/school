#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <signal.h>

void menu(char** args, int i);
void execProcess(char **args, int numArgs);
void childHandler(int signal, siginfo_t *signalInfo, void *hold);
int checkPipe(char **args, int numArgs);
int checkInRedir(char **args, int numArgs);
int checkOutRedir(char **args, int numArgs);
int checkBackground(char **args, int numArgs);
int outRedir(char **args, int numArgs);
int piping(char **args, int numArgs);

int main(int argc, char * argv[]){

    char buffer[500];
    char *args[20];
    char *token;
    int i;
    int isBackground;
        struct sigaction sig;
        sig.sa_sigaction = childHandler;

    do{
        int status = 0;
        isBackground = 0;

        printf("> ");
        fgets(buffer, 500, stdin);

        //checks if the command is exit
        if (strcmp(buffer, "exit\n")==0){
            printf("logout\n\n[Process completed]\n");
            exit(0);
        }

        token = strtok(buffer, " \n");
        i = 0;
        // parse the command line input
        while(token!=NULL){
            args[i]= malloc((strlen(token)+1)*sizeof(char));
            strcpy(args[i], token);
            token = strtok(NULL, " \n");
            i++;
        }
        args[i] = NULL;

        pid_t pid = fork(); //create a child process
        int type = 0;

        if (checkBackground(args,i) == 1){
            args[i-1] = NULL;
            isBackground = 1;
        }

        if (pid == 0){
            menu(args, i);
            exit(0);
        }else if(pid > 0){
            if (isBackground == 1)
                type = WNOHANG;
            waitpid(pid,&status,type);
            sigaction(SIGCHLD,&sig,NULL);
        }else {
            printf("Fork Error");
        }

        int j=0;
        for (j=0;j<i;j++){
            free(args[j]);
        }
    }while(1);
    return 0;
}

/*
 * Checks for any operators in the command line input
 */
void menu(char** args, int i){
    
    if (checkPipe(args, i)==1){
        piping(args, i);
    }else if (checkInRedir(args, i)==1){
        printf("<");
    }else if (checkOutRedir(args, i)==1){
        outRedir(args,i);
    }else{
        execProcess(args, i);
    }
}

/*
 * Executes the users command
 */
void execProcess(char **args, int numArgs){

    int test;
    char tempStr[20];
    strcpy(tempStr, args[0]);

    //if the process is an executable
    if (tempStr[0] == '.' && tempStr[1] == '/' ){

        int len = strlen(tempStr);
        char execName[len];
        int i =0;
        //take the first char* out of the array
        char * newArgs[15];
        i=0;
        for (i=0;i<numArgs-1;i++){
            newArgs[i] = args[i+1];
        }
        newArgs[i] = NULL;

        execvp(tempStr, newArgs);

    }else{
        char *path = "/bin/";
        char fullPath[20];

        strcpy(fullPath, path);
        strcat(fullPath, args[0]);
        test = execvp(fullPath, args);
        if (test == -1)
            printf("%s: command not found\n",args[0]);
    }
}

void childHandler(int signal, siginfo_t* signalInfo, void *hold){

    int status = 0;
    printf("process complete");
    if (signal == SIGCHLD)
        waitpid(signalInfo->si_pid,&status,0);
}

/*returns 1 if there is a |*/
int checkPipe(char **args, int numArgs){

    int i = 0;
    for (i=0;i<numArgs;i++){
        if (strcmp(args[i], "|")==0){
            return 1;
        }
    }
    return 0;
}

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

/*returns 1 if ther is a &*/
int checkBackground(char **args, int numArgs){

    int i = 0;
    for (i=0;i<numArgs;i++){
        if (strcmp(args[i], "&")==0){
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
        printf("No file\n");
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
        printf("File not found\n");
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
        printf("No file\n");
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
        printf("File not found\n");
        return 0;
    }
    
    fgets(buffer, 200, fp);
    
     //add the read in string to the end of the char* array
        token = strtok(buffer, " \n");
        n = j;
        while(token!=NULL){
            strcpy(newArgs[n], token);
            token = strtok(NULL, " \n");
            n++;
        }
        newArgs[n] = NULL;
    
    
    //send to get executed
    execProcess(newArgs, j);
    //close file
    fclose(fp)
    
    return 0;
}

int piping (char **args, int numArgs){

    int i = 0;
    int j = 0;
    char * pipeOut[5];
    char * pipeIn[100];

    int fd[2];
    int pipeError;
    char buffer[200];
    char *token;

    /* determines where in the array the | is */
    for (i=0;i<numArgs;i++){
        if (strcmp(args[i], "|")==0)
            break;
    }// i give the position of the |

    /* the stuff before the | goes into pipeOut */
    for (j=0;j<i;j++){
        pipeOut[j] = args[j];
    }
    pipeOut[j] = NULL;

    /* the stuff after the | goes into pipeIn */
    int m = i+1; //start with the element after the |
    int k = 0;
    for (m=i+1;m<numArgs;m++ ){
        pipeIn[k] = args[m];
        k++;
    }
    pipeIn[k] = NULL;
    
    /* make sure pipe worked */
    pipeError = pipe(fd);
    if (pipeError ==-1){
        printf("Pipe Error\n");
        return 0;
    }

    // make a new process to execute the first command	
    pid_t childpid = fork(); 

    if (childpid < 0)
        printf("Failed\n");
    else if (childpid == 0){

        close(1);//close stdout
        close(fd[0]);
        //switch the output, anything printed to stdout goes to the pipe
        dup2(fd[1], 1);
        /*send the pipeOut array, and its size to the execProcess function */ 
        execProcess(pipeOut,j); 

        exit(0);

    }else {
        close(0);//close stdin
        close(fd[1]);
        read(fd[0],buffer,200);
        
        //add the read in string to the end of the char* array
        token = strtok(buffer, " \n");
        n = k;
        while(token!=NULL){
            strcpy(pipeIn[n], token);
            token = strtok(NULL, " \n");
            n++;
        }
        pipeIn[n] = NULL;
        execvp(pipeIn, n);
    }
    close(fd[0]);
    close(fd[1]);
    return 0;
}
