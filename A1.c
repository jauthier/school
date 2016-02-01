#import <stdio.h>
#import <stdlib.h>
#import <string.h>

int checkPipe(char *str);
int checkInRedir(char *str);
int checkOutRedir(char *str);

int main(int argc, char argv[]){
    
    char * string1 = "ls -a | cat"
    
    if (checkPipe(string1)==1){
        
        
    }else if (checkInRedir(string1)==1){
        
    }else if (checkOutRedir(string1)==1){
        
    }
    
}

int checkPipe(char *str){
    
    int len = strlen(str);
    int i = 0;
    char letter;
    
    for (i=0;1<len;1++){
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
    
    for (i=0;1<len;1++){
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
    
    for (i=0;1<len;1++){
        letter = str[i];
        if (letter == '>'){
            return 1;
        }
    }
    return 0;
}

