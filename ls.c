#include <ls.h>

int lsMenu (char ** args, int numArgs){
    if (numArgs == 1){//no arguments
        lsNoArgs();
    }else if (strcmp(args[1],"-a")==0){
        lsAll();
    }
    return 0;
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
	return 0;
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
	return 0;

}
