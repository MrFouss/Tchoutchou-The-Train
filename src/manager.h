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
#include <stdbool.h>

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

/**
 * Checks if there is any train heading the opposit direction in the garage.
 */
bool canPassGarage(Train train);

/**
 * Checks if there is any train heading the opposit direction on the ligne.
 */
bool canPassLigne(Train train);

/**
 * sets direction counters according to the train assuming it has been allowed to pass.
 */
void setDirectionCounters(Train t);

#endif
