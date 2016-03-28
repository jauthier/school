#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

int ls();
void pipes();
int commandMenu(char *command, char **args);

int main(int argc, char* argv[]){

	char buffer[100];
	char * command;
	char * arguments[8];
	char * token;
	int done = 0; //0 = not done, 1 = done
	int i=0;

	while(done == 0){
		printf("$ ");
		fgets(buffer, 100, stdin);


		command = strtok(buffer, " \n");//the first on is alway the command

		token = strtok(NULL, " \n");

		while (token != NULL){
			//printf("%s",token);
			arguments[i]=token;
			i++;
			token = strtok(NULL," \n");
		}
		done = commandMenu(command, arguments);
	}

	return 0;
}

int commandMenu(char * command, char * args[8]){
	
	if (strcmp(command, "exit")==0){
		return 1;
	}else if (strcmp(command, "kill")==0){
		return 1;
	}else if(strcmp(command, "pipe")==0){
		pipes();
	}else if (strcmp(command, "ls")==0){
		ls(" ");
	}

	else {
		printf("%s: command not found\n",command);
	}

	return 0;
}

checkPipe(buffer)==1int ls(char * arg){

	struct dirent *dirEntry;
	char cwd[200];
	getcwd(cwd, sizeof(cwd));
	DIR *dir = opendir(cwd);

	if (dir == NULL){
		printf("\n");
		return 0;
	}
	/*printf("%s",args[0]);
	if (args[0]==NULL){
		while((dirEntry = readdir(dir)) != NULL){
			if (dirEntry->d_name[0]!='.') {
				printf("%s\n",dirEntry->d_name);
			}
		}
	}*/
	closedir(dir);

	return 0;
}

int forking(){
	int childpid;


}



/*Pipes

output of one program to be the input of another program

file descriptors - 0=in(stdin), 1=out(stdout)
can be done in cmd line, but also in code

inside a process there is a file tale, this a table of the 
file descriptors

*/

void pipes(){

	int pipe1[2];
	int pipe2[2];
	int ret;
	char buffer[20];

	ret = pipe(pipe1);
	if (ret == -1){
		printf("didn't work");
	}

	//pipe(pipe2);

	pid_t childpid = fork();

	if (childpid == 0){
		printf("I am the child\n");
		write(pipe1[1], "Hello there!",12);
		//check to see if it worked

	} else {
		printf("I am the parent\n");
		read(pipe1[0],buffer,15);
		printf("buf: %s",buffer);

	}

	


}







