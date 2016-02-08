#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

int checkPipe(char *str);
int checkInRedir(char *str);
int checkOutRedir(char *str);
int checkBackground(char *str);


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
		if (strcmp(buffer, "exit")==0){
			printf("logout\n\n[Process completed]");
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
		
		if (pid == 0){
			printf("In child\n");
			
			if (checkBackground(buffer) == 1){
				args[i-1] = NULL;
				isBackground = 1;
			}
			printf("%d\n",isBackground);
			
			
		} else if (pid > 0){
			printf("In parent\n");
		}else {
			printf("Fork Failed\n")
		}
		
		
		
		
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





