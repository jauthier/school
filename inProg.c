#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]){

	int i = 1;
	if (argc > 1){
		for (i=1;i<argc;i++){
			printf("%s\n", argv[i]);
		}
	} else
		printf("no arguments were found\n");

	return 0;
}
