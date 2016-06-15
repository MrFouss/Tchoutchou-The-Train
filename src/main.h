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
#include "communication.h"

/**
 * Called when an error occurred
 */
void exitProgram();

/**
 * Handler for the SIGINT signal
 */
void handlerSIGINT(int num);

#endif