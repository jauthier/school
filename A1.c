#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

int commandMenu(char ** args, int numArgs);
int lsNoArgs();
int checkPipe(char *str);
int checkInRedir(char *str);
int checkOutRedir(char *str);

int main(int argc, char argv[]){

    char buffer[100];
    char * args[8];
    int done = 0;
    char *token;
    int i=0;

    while (done == 0){

		i = 0;
        printf("> ");
        fgets(buffer, 100, stdin);
		printf("%s",buffer);
        token = strtok(buffer, " \n");
        while (token!= NULL){
            args[i]= malloc((strlen(token)+1)*sizeof(char));
            strcpy(args[i], token);
            token = strtok(NULL, " \n");
            i++;
        }
        
        if (checkPipe(buffer)==1){
            
        } else if (checkInRedir(buffer)==1){
             
        } else if (checkOutRedir(buffer)==1){

        } else {
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
    

	if (numArgs == 0){
		return 0;
	}
    if (strcmp(args[0],"exit")==0){
        printf("logout\n\n[Process completed]\n");
        exit(0);
    } else if (strcmp(args[0],"ls")==0){
		if (numArgs == 1){//no arguments
            lsNoArgs();
        }else if (strcmp(args[1],"-a")==0){
            lsAll();
		}

        
    }
    
}

int lsNoArgs(){

    struct dirent * dirEntry;
    char cwd[200];
    getcwd(cwd, sizeof(cwd));
    DIR *dir = opendir(cwd);

    if (dir == NULL){
        printf("\n");
        return 0;
    }

    while ((dirEntry = readdir(dir)) != NULL) {
        if (dirEntry->d_name[0] != '.') {
            printf("%s\n",dirEntry->d_name);
        }
    }
}

int lsAll(){
    struct dirent * dirEntry;
    char cwd[200];
    getcwd(cwd, sizeof(cwd));
    DIR *dir = opendir(cwd);

    if (dir == NULL){
        printf("\n");
        return 0;
    }

    while ((dirEntry = readdir(dir)) != NULL) {
        printf("%s\n",dirEntry->d_name);
    }

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

