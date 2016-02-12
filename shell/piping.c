
#include "piping.h"

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

int piping (char **args, int numArgs){

    int i = 0;
    int j = 0;
    char * procOutput[10];
    char * pipeIn[100];

    int fd[2];
    int pipeError;
    char buffer[500];
    char *token;

    /* determines where in the array the | is */
    for (i=0;i<numArgs;i++){
        if (strcmp(args[i], "|")==0)
            break;
    }// i give the position of the |

    /* the stuff before the | goes into procOutput */
    for (j=0;j<i;j++){
        procOutput[j] = args[j];
    }
    procOutput[j] = NULL;

    /* the stuff after the | goes into pipeIn */
    int m = i+1; //start with the element after the |
    int k = 0;
    for (m=i+1;m<numArgs;m++ ){
        pipeIn[k] = args[m];
        k++;
    }
    //pipeIn[k] = NULL;
    
    /* make sure pipe worked */
    pipeError = pipe(fd);
    if (pipeError ==-1){
        printf(": Pipe Error\n");
        return 0;
    }

    // make a new process to execute the first command	
    pid_t childpid = fork(); 

    if (childpid < 0){
        printf(": Fork Failed\n");
		return 0;
	}else if (childpid == 0){

        close(1);//close stdout
        close(fd[0]);
        //switch the output, anything printed to stdout goes to the pipe
        dup2(fd[1], 1);
        /*send the pipeOut array, and its size to the execProcess function */ 
        execProcess(procOutput,j);

        exit(0);


    }else {
        wait(NULL);
        close(0);//close stdin
        close(fd[1]);
        read(fd[0],buffer,500);

        int len = strlen(buffer);
        int q = 0;
        for (q=0;q<len;q++){
            if (buffer[q]=='\n'){
                buffer[q]='\0';
                break;
            }
        }

        //add the read in string to the end of the char* array
        token = strtok(buffer, " \n");

        while(token!=NULL){
            pipeIn[k] = token;
            k++;
            token = strtok(NULL, " \n\0");
        }
        pipeIn[k] = NULL;
        execProcess(pipeIn, k);
    }
    close(fd[0]);
    close(fd[1]);
    return 0;
}