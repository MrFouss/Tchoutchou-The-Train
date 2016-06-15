#ifndef MANAGER_H
#define MANAGER_H

#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "communication.h"
#include "main.h"

/*************************/
/* STRUCTURES DEFINITION */
/*************************/

typedef struct ManagerThreadArg {
	char name[30];
	int id;
	long permissionFreq; /* in microseconds */
	MessageList* messageList;
} ManagerThreadArg;

/**************/
/* PROTOTYPES */
/**************/

/**
 * Called to interrupt process (signal handler)
 */
void managerHandlerSIGINT(int num);

/**
 * Critical zone manager thread function
 */
void* managerThread(void* arg);

/**
 * Called to free allocations and properly terminate the process
 */
void exitManager(int num);

/**
 * Initializes the manager variables and lauch manager threads
 */
int initManager();

/**
 * Function called to initialize a manager thread and terminate it eventually
 */
void processManager(int global_msqid);

#endif
