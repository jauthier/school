
#include "execute.h"

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