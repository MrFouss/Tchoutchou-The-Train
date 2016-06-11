#ifndef MANAGER_H
#define MANAGER_H

#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <mqueue.h>

/**********/
/* MACROS */
/**********/

#define QNAME "/iLikeTrains"
#define MAXMSG 50
#define MSGSIZE 256

/********************/
/* GLOBAL VARIABLES */
/********************/

pthread_t P0, P1, P2, P3;
mqd_t MANAGER_MSQID, MANAGER_GLOBAL_MSQID;

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
