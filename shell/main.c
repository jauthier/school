#include "main.h"

char rootDir[100];

int main(int argc, char * argv[]){

    char buffer[500];
    char *args[20];
    char *token;
    int i;
    int isBackground;
    struct sigaction sig;
    sig.sa_sigaction = childHandler;
    char *cwd = malloc(sizeof(char)*100);
	getcwd(rootDir, 100);

    do{
        int status = 0;
        isBackground = 0;
        
        cwd = getCWD();

        printf("%s> ",cwd);
        fgets(buffer, 500, stdin);

		if (strcmp(buffer, "\n")==0)
			continue;

		writeHistory(buffer);

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
            strcpy( args[i], token);
            token = strtok(NULL, " \n");
            i++;
        }
        args[i] = NULL;

        if (strcmp(args[0],"cd")==0){
            if (i==1){
                strcpy(cwd, rootDir);
				chdir(cwd);
            } else {
                cwd = changeCWD(args[1]);
            }
			continue;
        }
		if (strcmp(args[0],"history")==0){
			if (args[1] == NULL)
				readHistory();
			else if (strcmp(args[1],"-c")==0)
				clearHistory();
			else
				readnEntries(args[1]);
			continue;
		}

        pid_t pid = fork(); //create a child process
        int type = 0;

        if (checkBackground(args,i) == 1){
            args[i-1] = NULL;
            isBackground = 1;
        }

        if (pid == 0){
            menu(args, i);
            exit(0);
        }else if(pid > 0){
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
