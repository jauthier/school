
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <signal.h>

#include <piping.h>
#include <history.h>
#include <redirection.h>
#include <execute.h>

int writeHistory(char *toWrite);
void readHistory();
void clearHistory();
int readnEntries(cahr* arg);
char *getCWD();
char *changeCWD(char* dir);
void menu(char** args, int i);
int execProcess(char **args, int numArgs);
void childHandler(int signal, siginfo_t *signalInfo, void *hold);
int checkBackground(char **args, int numArgs);

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
            strcpy( args[i], token);
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
        printf("%S: No such file or direcotry\n");

    fprintf(fp,"");
    
    fclose(fp);
    free(fileName);
}

int readnEntries(char *arg){
    
    char *endPtr;
    
    if (strtol(arg, &endPtr,10) == 0){
        printf("%s: command not found\n",arg);
        return 0;
    } else{
        
        int numEntries = strtol(arg, &endPtr,10);
        char *fileName = malloc(sizeof(char)*100);
        strcpy(fileName, rootDir);
        strcat(fileName,"/");
        strcat(fileName,".CIS3110_history");
        FILE* fp;
        fp = fopen(fileName,"w");
        if (fp == NULL)
            printf("%S: No such file or direcotry\n");
        
        int count = 1;
        while ((fgets(buffer,100,fp)!= NULL) && (count != numEntries)){
            printf(" %d  %s\n",count,buffer);
            count++;
        }
        fclose(fp);
        free(fileName);
    }
    return 0;
}

int checkBackground(char **args, int numArgs){

    int i = 0;
    for (i=0;i<numArgs;i++){
        if (strcmp(args[i], "&")==0){
            return 1;
        }
    }
    return 0;
}

void childHandler(int signal, siginfo_t* signalInfo, void *hold){

    int status = 0;
    if (signal == SIGCHLD)
        waitpid(signalInfo->si_pid,&status,0);
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

