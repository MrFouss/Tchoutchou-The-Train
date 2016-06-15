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

/********************/
/* GLOBAL VARIABLES */
/********************/

/* manager threads, 1 per critial zone */
pthread_t AIGUILLAGE1, AIGUILLAGE2, TUNNEL;

/* global message queue id to communication with trains */
int MANAGER_GLOBAL_MSQID;

/* list of messages used by manager threads to store messages */
MessageList A1MSG, A2MSG, TMSG;

/**************/
/* PROTOTYPES */
/**************/

/**
 * Called to interrupt process (signal handler)
 */
void managerSIGINThandler(int num);

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
