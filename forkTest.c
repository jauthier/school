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
int writeHistory(char *toWrite);
void readHistory();
void clearHistory();
char *getCWD();
char *changeCWD(char* dir);
int execProcess(char **args, int numArgs);
void childHandler(int signal, siginfo_t *signalInfo, void *hold);
int checkPipe(char **args, int numArgs);
int checkInRedir(char **args, int numArgs);
int checkOutRedir(char **args, int numArgs);
int checkBackground(char **args, int numArgs);
int outRedir(char **args, int numArgs);
int inRedir(char **args, int numArgs);
int piping(char **args, int numArgs);

char rootDir[100];

int main(int argc, char * argv[]){

    char buffer[500];
    char *args[20];
    char *token;
    int i;
    int isBackground;
    struct sigaction sig;
    sig.sa_sigaction = childHandler;
    char *cwd = malloc(sizeof(char)*100);
	getcwd(rootDir, 100);

    do{
        int status = 0;
        isBackground = 0;
        
        cwd = getCWD();

        printf("%s> ",cwd);
        fgets(buffer, 500, stdin);

		if (strcmp(buffer, "\n")==0)
			continue;

		writeHistory(buffer);

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

        if (strcmp(args[0],"cd")==0){
            if (i==1){
                strcpy(cwd, rootDir);
				chdir(cwd);
            } else {
                cwd = changeCWD(args[1]);
            }
			continue;
        }
		if (strcmp(args[0],"history")==0){
			if (args[1] == NULL)
				readHistory();
			else if (strcmp(args[1],"-c")==0)
				clearHistory();
			continue;
		}

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

int writeHistory(char *toWrite){

	char *fileName = malloc(sizeof(char)*100);
	strcpy(fileName, rootDir);
	strcat(fileName,"/");
	strcat(fileName,".CIS3110_history");
	FILE* fp;
	fp = fopen(fileName,"a");
	if (fp == NULL){
		printf("%S: No such file or direcotry\n");
		return 0;
	}
	fprintf(fp,"%s",toWrite);

	fclose(fp);
	free(fileName);
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
		printf("%S: No such file or direcotry\n");

	int count = 1;
	while (fgets(buffer,100,fp)!= NULL){
	    printf(" %d  %s\n",count,buffer);
	    count++;
	}
	fclose(fp);
}

void clearHistory(){


}


char *getCWD(){
    char *temp = malloc(sizeof(char)*100);
    getcwd(temp,100);
    
    return temp;
}

char* changeCWD(char* dir){
    char *cwd = malloc(sizeof(char)*100);
	getcwd(cwd,100);
    int len = strlen(cwd)-1;
    if (strcmp(dir, "..")==0){
        char letter = cwd[len]; //last char of string
        while (letter != '/'&&len != 0){
            cwd[len] = '\0';
            len--;
            letter = cwd[len];
        }
        cwd[len] = '\0';
    	chdir(cwd);
    } else {
        strcat(cwd,"/");
        strcat(cwd,dir);
    
    	int catch = chdir(cwd);
	    if (catch==-1)
        	printf("%s: No such file or directory\n",dir);

    }
    return cwd;
}

/*
 * Checks for any operators in the command line input
 */
void menu(char** args, int i){
    
    if (checkPipe(args, i)==1){
        piping(args, i);
    }else if (checkInRedir(args, i)==1){
        inRedir(args, i);
    }else if (checkOutRedir(args, i)==1){
        outRedir(args,i);
    }else{
        execProcess(args, i);
    }
}

/*
 * Executes the users command
 */
int execProcess(char **args, int numArgs){

    int test;
    char tempStr[20];
    strcpy(tempStr, args[0]);

	if (strcmp(args[0],"sort")==0){
		int catch = execvp(args[0],args);
		if (catch ==-1)
			printf("%s: No such file or directory\n",args[1]);
		return 0;;
	}else

    //if the process is an executable
    if (tempStr[0] == '.' && tempStr[1] == '/' ){

        int i =0;
        //take the first char* out of the array
        char * newArgs[15];
        i=0;
        for (i=0;i<numArgs-1;i++){
            newArgs[i] = args[i+1];
        }
        newArgs[i] = NULL;

        test = execvp(tempStr, newArgs);
		if (test ==-1)
			printf("%s: No such file or directory\n",tempStr);

    }else{
        char *path = "/bin/";
        char fullPath[20];

		if (args[1] != NULL){
			if (strcmp(args[1],"$HISTFILE")==0){
				char *pathStr = malloc(sizeof(char)*100);
				strcpy(pathStr,rootDir);
				strcat(pathStr,"/");
				strcat(pathStr,".CIS3110_history");
				args[1]=pathStr;

			}else if (strcmp(args[1],"$HOME")==0){
				strcpy(args[1],rootDir);
				printf("hist");
			}
		}
        strcpy(fullPath, path);
        strcat(fullPath, args[0]);
		test = execvp(fullPath, args);
        if (test == -1)
            printf("%s: command not found\n",args[0]);
    }
    return 0;
}

void childHandler(int signal, siginfo_t* signalInfo, void *hold){

    int status = 0;
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

int piping (char **args, int numArgs){

    int i = 0;
    int j = 0;
    char * procOutput[10];
    char * pipeIn[100];

    int fd[2];
    int pipeError;
    char buffer[500];
    char *token;

    /* determines where in the array the | is */
    for (i=0;i<numArgs;i++){
        if (strcmp(args[i], "|")==0)
            break;
    }// i give the position of the |

    /* the stuff before the | goes into procOutput */
    for (j=0;j<i;j++){
        procOutput[j] = args[j];
    }
    procOutput[j] = NULL;

    /* the stuff after the | goes into pipeIn */
    int m = i+1; //start with the element after the |
    int k = 0;
    for (m=i+1;m<numArgs;m++ ){
        pipeIn[k] = args[m];
        k++;
    }
    //pipeIn[k] = NULL;
    
    /* make sure pipe worked */
    pipeError = pipe(fd);
    if (pipeError ==-1){
        printf(": Pipe Error\n");
        return 0;
    }

    // make a new process to execute the first command	
    pid_t childpid = fork(); 

    if (childpid < 0){
        printf(": Fork Failed\n");
		return 0;
	}else if (childpid == 0){

        close(1);//close stdout
        close(fd[0]);
        //switch the output, anything printed to stdout goes to the pipe
        dup2(fd[1], 1);
        /*send the pipeOut array, and its size to the execProcess function */ 
        execProcess(procOutput,j);

        exit(0);

    }else {
        wait(NULL);
        close(0);//close stdin
        close(fd[1]);
        read(fd[0],buffer,500);

        int len = strlen(buffer);
        int q = 0;
        for (q=0;q<len;q++){
            if (buffer[q]=='\n'){
                buffer[q]='\0';
                break;
            }
        }

        //add the read in string to the end of the char* array
        token = strtok(buffer, " \n");

        while(token!=NULL){
            pipeIn[k] = token;
            k++;
            token = strtok(NULL, " \n\0");
        }
        pipeIn[k] = NULL;
        execProcess(pipeIn, k);
    }
    close(fd[0]);
    close(fd[1]);
    return 0;
}
