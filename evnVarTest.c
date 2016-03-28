#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int  main (){

	char buffer[30];
	char *cwd = malloc(sizeof(char)*100);

	getcwd(cwd, 100);
	//cwd = malloc((strlen(buffer)+1)*sizeof(char));
	//strcpy(cwd, buffer);

	int len = strlen(cwd);

	printf("%s  is %d long\n",cwd, len);

	printf("lets change the dir\n");

	strcat(cwd,"/new");
	int check = chdir(cwd);
	if (check == -1)
		printf("problem\n");

	char * newDir = NULL;
	getcwd(newDir, 100);

	printf("%s\n",newDir);


	return 0;
}
