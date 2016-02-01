#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int checkPipe(char *str);
int checkInRedir(char *str);
int checkOutRedir(char *str);

int main(int argc, char argv[]){

    char buffer[100] = "ls -a | cat";
    char * args[8];
	int done = 0;
	int i=0;
	char *token;

	while (done == 0){

		token = strtok(buffer, " \n");
		while (token!= NULL){
			args[i]= malloc(strlen(token)+1);
			strcpy(args[i], token);
			printf("%s\n",args[i]);
			token = strtok(NULL, " \n");
			i++;
		}


    	if (checkPipe(buffer)==1){
        
    	}else if (checkInRedir(buffer)==1){
        
    	}else if (checkOutRedir(buffer)==1){
        
    	}
		done =1;
	}
    return 0;
}

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

