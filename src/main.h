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

//parent process
pid_t program;
//children process pid
pid_t manager, train;
//inter-process message queue
int msqid;

#endif