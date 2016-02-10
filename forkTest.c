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

		pid_t pid = fork();
		int type = 0;
		
		if (checkBackground(args,i) == 1){
			args[i-1] = NULL;
			isBackground = 1;
		}
		
		if (pid == 0){
			
			printf("In Child\n");
			menu(args, i);
			exit(0);
			
		}else if(pid > 0){
			
			printf("In parent");
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

void menu(char** args, int i){
	printf("In Menu\n");
	fflush(stdout);

	if (checkPipe(args, i)==1){
		printf("|\n");
		piping(args, i);

	}else if (checkInRedir(args, i)==1){
		printf("<");
	}else if (checkOutRedir(args, i)==1){
		outRedir(args,i);
	}else{
		execProcess(args, i);
	}
	printf("Menu end\n");
}

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


		printf("\n%s\n",execName);


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
	execProcess(newArgs, numArgs);
	//close file
	fclose(fp);

	return 0;
}

//int inRedir()

int piping (char **args, int numArgs){

	int i = 0;

	char * pipeOut[5];
	char * pipeIn[100];

	int pipe1[2];
	int pipeError;
	char buffer[200];


	//determines where in the array the | is
	for (i=0;i<numArgs;i++){
		if (strcmp(args[i], "|")==0)
			break;
	}// i give the position of the |

	//the stuff before the | goes into pipeOut
	int j = 0;
	for (j=0;j<i;j++){
		pipeOut[j] = args[j];
		printf("%d: %s\n",j,pipeOut[j]);
	}
	pipeOut[j] = NULL;

	//the stuff after the | goes into pipeIn
	int m = i+1; //start with the element after the |
	int k = 0;
	for (m=i+1;m<numArgs;m++ ){
		pipeIn[k] = args[m];
		printf("%d: %s\n",k,pipeIn[k]);
		k++;
	}
	pipeIn[k] = NULL;


	pipeError = pipe(pipe1);
	pid_t childpid = fork();

	if (childpid < 0)
		printf("Failed\n");
	else if (childpid == 0){

		close(1);
		close(pipe1[0]);
		//switch the output, anything printed to stdout goes to the pipe
		dup2(pipe1[1], 1);
		execProcess(pipeOut,j);

		exit(0);

	}else {
		close(0);
		close(pipe1[1]);
		read(pipe1[0],buffer,200);
		printf("%s\n",buffer);
		//add the read in string to the end of the char* array





	}




	return 0;
}
