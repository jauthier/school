#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <ctype.h>

int main(int argc, char * argv[]){
	
	char buffer[500];
	char *args[10];
	char *path = "/bin/";
	char fullPath[20];
	char *token;
	int i;
	int isBackground;
	
	
	do{
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
			args[i] = token;
			token = strtok(NULL, " \n");
			i++;
		}
		args[i] = NULL;
		
		pid_t pid = fork();
		int type = 0;
		int status = 0;
		
		if (checkBackground(args,i) == 1){
			args[i-1] = NULL;
			isBackground = 1;
		}
		
		if (pid >= 0){
			if (pid == 0){
				printf("In Child");
				//menu(args,i);
			}else{
				printf("In Parent\n");
				if (isBackground==1)
					type = WNOHANG;
				waitpid(pid, &status, type);
				if (type == 0)
					printf("in forground\n");
			}
			
		}else {
			printf("Fork Failed\n");
		}
	}while (1);
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
