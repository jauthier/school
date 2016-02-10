#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <ctype.h>

void menu(char** args, int i);
void execProcess(char **args);
int checkPipe(char **args, int numArgs);
int checkInRedir(char **args, int numArgs);
int checkOutRedir(char **args, int numArgs);
int checkBackground(char **args, int numArgs);
int outRedir(char **args, int numArgs);

int main(int argc, char * argv[]){
	
	char buffer[500];
	char *args[10];
	char *token;
	int i;
	int numArgs;
	//int isBackground;
	
	do{
		int status = 0;
		//isBackground = 0;
		
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
			args[i]= malloc((strlen(token)+1)*sizeof(char));
            strcpy(args[i], token);
			token = strtok(NULL, " \n");
			i++;
		}
		args[i] = NULL;
		//numArgs = i;
		
		pid_t pid = fork();
		
		if (pid == 0){
			
			printf("In Child\n");
			sleep(1);
			printf("In Menu\n");

			if (checkPipe(args, i)==1){
				printf("|");
			}
			if (checkInRedir(args, i)==1){
				printf("<");
			}
			printf("Here now");
			if (checkOutRedir(args, i)==1){
				outRedir(args,i);
			}else{
				printf("cat");
				execProcess(args);
			}
			printf("end");
			exit(status);
			
		}else if(pid > 0){
			
			printf("In parent");
			sleep(5);
			waitpid(pid,&status,0);
			sleep(2);
		}else {
			printf("Fork Error");
		}
		
		
		
	}while(1);
	
	int j=0;
    for (j=0;j<i;j++){
		free(args[j]);
	}
	
	return 0;
}

void menu(char** args, int i){
	
}

void execProcess(char **args){

	char *path = "/bin/";
	char fullPath[20];

	strcpy(fullPath, path);
	strcat(fullPath, args[0]);
	execvp(fullPath, args);
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
	printf("cat");
	for (i=0;i<numArgs;i++){
		if (strcmp(args[i], ">")==0){
			//printf("here");
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
	//FILE *fp;

	for (i=0;i<numArgs;i++){
		if (strcmp(args[i], ">")==0)
			break;
	}

	if (args[i+1] != NULL){
		strcpy(outFile, args[i+1]);
	} else {
		printf("No file.\n");
		return 0;
	}

	printf("i: %d, i-1 arg: %s\n",i,args[i-1]);

	for (j=0;j<i;j++){
		newArgs[j] = args[j];
		printf("\n----%s----\n", newArgs[j]);
	}
	newArgs[j+1] = NULL;
	if (newArgs[j+1]==NULL)
		printf("last one good\n");
	
	execProcess(newArgs);

	/* char path[20] = "/bin/"; */
	/* strcat(path, newArgs[0]); */
	/* printf("Path: %s\n",path); */
	//fp = freopen(outFile,"w",stdout);
	//execvp("/bin/ls", newArgs);
	
	//fclose(fp);

	return 0;
}
