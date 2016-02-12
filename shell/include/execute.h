#ifndef execute_h
#define execute_h

#include "main.h"

char *getCWD();
char *changeCWD(char* dir);
void menu(char** args, int i);
int execProcess(char **args, int numArgs);
void childHandler(int signal, siginfo_t *signalInfo, void *hold);
int checkBackground(char **args, int numArgs);

#endif
