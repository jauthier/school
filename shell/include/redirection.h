#ifndef redir_h
#define redir_h

#include "main.h"

int checkInRedir(char **args, int numArgs);
int checkOutRedir(char **args, int numArgs);
int outRedir(char **args, int numArgs);
int inRedir(char **args, int numArgs);

#endif
