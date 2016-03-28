#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#define TRUE 1;
#define FALSE 0;


void sigEnd(int sigNum, siginfo_t *signalInfo, void* hold);
int checkBackground(char *str);

int main(){

    char buffer[500];
    char *args[10];
    char *path = "/bin/";
    char fullPath[20];
    int numArgs;
	int inBackground;



    while (1){
		printf(">");
		fgets(buffer, 500, stdin);

		//checks if the command is exit
		if (strcmp(buffer, "exit")==0){
			printf("logout\n\n[Process completed]\n");
			exit(0);
		}

		char *token = strtok(buffer, " \n");
		int i = 0;
		//parse the comand line input
		while (token != NULL){
			args[i] = token;
			token = strtok(NULL, " \n");
			i++;
		}
		args[i] = NULL;
		numArgs = i;

		inBackground = 0;

		for (i=0;i<numArgs;i++){
			printf("%s",args[i]);
		}

		pid_t pid = fork();

		struct sigaction endSignal;
		endSignal.sa_sigaction = sigEnd;

		if (pid == 0){
			printf("\nIn child process\n");

			if (checkBackground(buffer)==1){
				inBackground = 1;
			}

			printf("%d",inBackground);
			if (inBackground == 1){
				execProcessInBack(args);
			} else{
				execProcess(args);
			}
		}else if (pid > 0){
			printf("\nIn parent process\n");
			wait(NULL);
			printf("\nChild exited\n");
			sigaction(SIGCHLD,&endSignal, NULL);

		}else {
			printf("Forking failed");
		}


    }
}	
int execProcess(char **args){
	char *path = "/bin/";
	char fullPath[20];

	strcpy(fullPath, path);
	strcat(fullPath, args[0]);
	execvp(fullPath, args);
}

int execProcessInBack(char **args){
	
}

void sigEnd(int sigNum, siginfo_t *signalInfo, void* hold){

	if (sigNum == SIGINT){
		printf("goodBye");
		exit(0);
	}
}

/*returns 1 if there is a |*/
int checkPipe(char *str){
    
    int len = strlen(str);
    int i = 0;
    char letter;
    
    for (i=0;i<len;i++){
        letter = str[i];
        if (letter == '|'){
            return 1;
        }
    }
    return 0;
}

/*returns 1 if ther is a <*/
int checkInRedir(char *str){
    
    int len = strlen(str);
    int i = 0;
    char letter;
    
    for (i=0;i<len;i++){
        letter = str[i];
        if (letter == '<'){
            return 1;
        }
    }
    return 0;
}

/*returns 1 if ther is a >*/
int checkOutRedir(char *str){
    
    int len = strlen(str);
    int i = 0;
    char letter;
    
    for (i=0;i<len;i++){
        letter = str[i];
        if (letter == '>'){
            return 1;
        }
    }
    return 0;
}

/*returns 1 if ther is a &*/
int checkBackground(char *str){

	int len = strlen(str);
	int i = 0;
	char letter;

	for (i=0;i<len;i++){
		letter = str[i];
		if (letter == '&'){
			return 1;
		}
	}
	return 0;
}
