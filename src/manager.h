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

/**********/
/* MACROS */
/**********/

#define QNAME "/iliketrains"
#define MAXMSG 20
#define MSGSIZE 256

/********************/
/* GLOBAL VARIABLES */
/********************/

pthread_t P0, P1, P2, P3;
int MANAGER_MSQID, MANAGER_GLOBAL_MSQID;

/**************/
/* PROTOTYPES */
/**************/

void* threadP0(void* arg);

void* threadP1(void* arg);

void* threadP2(void* arg);

void* threadP3(void* arg);

void exitManager(int num);

int initManager();

void processManager(int global_msqid);

#endif
