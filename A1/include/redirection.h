#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <signal.h>

int checkInRedir(char **args, int numArgs);
int checkOutRedir(char **args, int numArgs);
int outRedir(char **args, int numArgs);
int inRedir(char **args, int numArgs);
