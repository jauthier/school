#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <signal.h>

int writeHistory(char *toWrite);
void readHistory();
void clearHistory();
int readnEntries(cahr* arg);