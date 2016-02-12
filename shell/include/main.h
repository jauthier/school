#ifndef main_h
#define main_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <signal.h>

#include "piping.h"
#include "history.h"
#include "redirection.h"
#include "execute.h"

extern char rootDir[100];

#endif

