#ifndef MANAGER_H
#define MANAGER_H

#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <string.h>
#include "communication.h"
#include <string.h>

/**********/
/* MACROS */
/**********/

#define QNAME "/iliketrains"
#define MAXMSG 20
#define MSGSIZE 256

typedef struct ManagerThreadArg {
	char name[30];
	int id;
	long permissionFreq; /*microsecond*/
	MessageQueue* messageQueue;
} ManagerThreadArg;

/********************/
/* GLOBAL VARIABLES */
/********************/

pthread_t aiguillage1, aiguillage2, tunnel;
int MANAGER_GLOBAL_MSQID;
MessageQueue A1msg, A2msg, Tmsg;

/**************/
/* PROTOTYPES */
/**************/

void* managerThread(void* arg);

void exitManager(int num);

int initManager();

void processManager(int global_msqid);

#endif
