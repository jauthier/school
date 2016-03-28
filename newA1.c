#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <ls.h>

int commandMenu(char ** args, int numArgs);
int lsNoArgs();
int checkPipe(char *str);
int checkInRedir(char *str);
int checkOutRedir(char *str);

int main(int argc, char * argv[]){

    char buffer[100];
    char * args[8];
    int done = 0;
    char *token;
    int i=0;

    while (done == 0){

		i = 0;
        printf("> ");
        //read command line
		fgets(buffer, 100, stdin);
		//printf("%s",buffer);

		pid = fork();
		if (pid < 0){
			wait(NULL);
		} else if (pid > 0){
			//error
		} else {
		// we are in the child process
    	    //split up commands by space
			token = strtok(buffer, " \n");
        	while (token!= NULL){
            	args[i]= malloc((strlen(token)+1)*sizeof(char));
            	strcpy(args[i], token);
            	token = strtok(NULL, " \n");
            	i++;
        	}

		}


        
        if (checkPipe(buffer)==1){
            
        } else if (checkInRedir(buffer)==1){
             
        } else if (checkOutRedir(buffer)==1){

        } else if (checkBackground(buffer)==1){

		}else {
            commandMenu(args, i); //i is one more then there are args
        }
        
        
        int j=0;
        for (j=0;j<i;j++){
			free(args[j]);
		}
        
        
    }
    return 0;
}

int commandMenu(char ** args, int numArgs){
    
	//if there was no input
	if (numArgs == 0){
		return 0;
	}
	//
    if (strcmp(args[0],"exit")==0){
        printf("logout\n\n[Process completed]\n");
        exit(0);
    } else if (strcmp(args[0],"ls")==0){
        lsMenu(args, numArgs);

        
    }
    return 0;
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
