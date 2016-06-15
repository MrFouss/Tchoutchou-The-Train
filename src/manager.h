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
	MessageQueue* messageQueue;
} ManagerThreadArg;

/********************/
/* GLOBAL VARIABLES */
/********************/

pthread_t AIGUILLAGE1, AIGUILLAGE2, TUNNEL;
int MANAGER_GLOBAL_MSQID;
MessageQueue A1MSG, A2MSG, TMSG;

/**************/
/* PROTOTYPES */
/**************/

void handler(int num);

void* managerThread(void* arg);

void exitManager(int num);

int initManager();

void processManager(int global_msqid);

#endif
