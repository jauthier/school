#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){

    char buffer[500];
    char *args[10];
    char *path = "/bin/";
    char fullPath[20];
    int numArgs;

    while (1){
		printf(">");
		fgets(buffer, 500, stdin);

		char *token = strtok(buffer, " \n");
		int i = 0;

		while (token != NULL){
			args[i] = token;
			token = strtok(NULL, " \n");
			i++;
		}
		args[i] = NULL;
		numArgs = i;

		/*for (i=0;i<numArgs;i++){
			printf("%s",args[i]);
		}*/

		strcpy(fullPath, path);
		strcat(fullPath, args[0]);

		printf("%s",fullPath);

		int pid = fork();
		if (pid == 0){
			printf("\nIn child process\n");
			execvp(fullPath,args);

		}else {
			printf("\nIn parent process\n");
			wait(NULL);
			printf("\nChild exited\n");
		}


    }


}
