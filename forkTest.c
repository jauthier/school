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
	int numArgs;
	int isBackground;
	
	do{
		int status = 0;
		isBackground = 0;
		
		printf(">");
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
		numArgs = i;
		
		pid_t pid = fork();
		
		if (pid == 0){
			
			printf("In Child\n");
			sleep(1);
			exit(status);
			
		}else if(pid > 0){
			
			printf("In parent");
			waitpid(pid, &status ,0);
			
		}else {
			printf("Fork Error");
		}
		
		
		
	}while(1);
	
	return 0;
}