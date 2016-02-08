#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>


void forkInBack(char** args, int i);
void forkInFore(char** args, int i);
void execProcess(char **args);
int checkPipe(char **args, int numArgs);
int checkInRedir(char **args, int numArgs);
int checkOutRedir(char **args, int numArgs);
int checkBackground(char **args, int numArgs);


int main(int argc, char * argv[]){
	
	char buffer[500];
	char *args[10];
	char *path = "/bin/";
	char fullPath[20];
	char *token;
	int i;
	int isBackground;
	
	while(1){
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
		
		if (checkBackground(args,i) == 1){
			args[i-1] = NULL;
			isBackground = 1;
		}
			
		if (isBackground == 1)
			forkInBack(args, i);
		else
			forkInFore(args, i);
	}
}

void forkInBack (char** args, int i){
	int status;
	pid_t pid = fork();

	if (pid == 0){
		printf("In child\n");
		
		if (checkPipe(args, i)==1)
			printf(" ");
			
		else if (checkInRedir(args, i)==1)
			printf(" ");
			
		else if (checkOutRedir(args, i)==1)
			printf(" ");
			
		else
			execProcess(args);
		
	} else if (pid > 0){
		printf("In parent\n");
		waitpid(pid, &status, WNOHANG);
		
	}else {
		printf("Fork Failed\n");
	}
}

void forkInFore (char **args, int i){
	int status;
	pid_t pid = fork();
		
	if (pid == 0){
		printf("In child\n");
		
		if (checkPipe(args, i)==1)
			printf(" ");
		else if (checkInRedir(args, i)==1)
			printf(" ");
			
		else if (checkOutRedir(args, i)==1)
			printf(" ");
			
		else
			execProcess(args);
		
	} else if (pid > 0){
		printf("In parent\n");
		waitpid(pid, &status, 0);
		
	}else {
		printf("Fork Failed\n");
	}
	
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

void inRedir(){
	
	FILE * fp;
	char inFile[20];
	
	
	
	
}

void outRedir(char **args, int numArgs){
	
	int i = 0;
	char outFile[20];
	char *newArgs[5];
	FILE *fp;
	
	
	for (i=0;i<numArgs;i++){
		if (strcmp(args[i], ">")==0)
			break;
	}
	
	outFile = args[i+1];
	
	int j = 0;
	for (j=0;j<i;j++){
		strcpy(newArgs[j],args[j]);
	}
	
	fp = freopen(outFile,"w",stdout);
	
	execProcess(newArgs);
	
	
}

