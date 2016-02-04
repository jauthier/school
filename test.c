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
		
		//printf("%s",fullPath);

		int pid = fork();
		if (pid == 0){
			printf("\nIn child process\n");
			if (checkPipe(buffer)==1){
            
			} else if (checkInRedir(buffer)==1){
             
			} else if (checkOutRedir(buffer)==1){

			} else if (checkBackground(buffer)==1){

			}else {
				execProcess(args);
			}	
		}else if (pid > 0){
			printf("\nIn parent process\n");
			wait(NULL);
			printf("\nChild exited\n");
		}else {
			//error
			
		}


    }
}	
int execProcess(char **args){
	char *path = "/bin/";
	char fullPath[20];

	strcpy(fullPath, path);
	srtcat(fullPath, args[0]);
	execvp(fullPath, args);
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
