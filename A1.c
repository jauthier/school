#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

void commandMenu(char ** args, int numArgs);
int lsNoArgs();
int checkPipe(char *str);
int checkInRedir(char *str);
int checkOutRedir(char *str);

int main(int argc, char argv[]){

    char buffer[100];
    char * args[8];
    int done = 0;
    int i=0;
    char *token;

    while (done == 0){

        printf("> ");
        fgets(buffer, 100, stdin);

        token = strtok(buffer, " \n");
        while (token!= NULL){
            args[i]= malloc(strlen(token)+1);
            strcpy(args[i], token);
            printf("%s\n",args[i]);
            token = strtok(NULL, " \n");
            i++;
        }
        
        if (checkPipe(buffer)==1){
            
        } else if (checkInRedir(buffer)==1){
             
        } else if (checkOutRedir(buffer)==1){
            
        } else if (){
            
        } else {
            commandMenu(args, i); //i is one more then there are args
        }
        
        
        
        
        
        done =1;
    }
    return 0;
}

void commandMenu(char ** args, int numArgs){
    
    if (strcmp(args[0],"exit")==0){
        printf("logout\n\n[Process completed]");
        exit(0);
    } else if (strcmp(args[0],"ls")==0){
        if (i == 1){//no arguments
            
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
        if (dirENtry->d_name[0] != '.') {
            printf("%s\n",dirEntry->d_name);
        }
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

