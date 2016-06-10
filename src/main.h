#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include "train.h"
#include "manager.h"
#include "network.h"

/********************/
/* GLOBAL VARIABLES */
/********************/

// Parent process
pid_t PROGRAM;

// Children process PID
pid_t MANAGER, TRAIN;

// Inter-process message queue
int MSQID;

#endif