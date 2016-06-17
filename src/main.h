#ifndef MAIN_H
#define MAIN_H

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#include <unistd.h>
#endif

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "manager.h"
#include "train.h"
#include "header_master.h"

/**************/
/* PROTOTYPES */
/**************/

/* Called when an error occurred */
void exitProgram();

/* Handler for the SIGINT signal */
void handlerSIGINT(int num);

#endif