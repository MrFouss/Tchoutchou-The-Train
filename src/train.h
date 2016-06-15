#ifndef TRAIN_H
#define TRAIN_H

#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/msg.h>
#include <bits/sigthread.h>

#include "communication.h"
#include "parser.h"

/*************************/
/* STRUCTURES DEFINITION */
/*************************/

/*
typedef struct TrainNode {
    pthread_t thread;
    struct TrainNode* next;
} TrainNode;

typedef struct TrainNode* TrainList;
*/

/********************/
/* GLOBAL VARIABLES */
/********************/

pthread_t* TRAINS;
int TRAIN_NBR;
int TRAIN_GLOBAL_MSQID;

/**************/
/* PROTOTYPES */
/**************/

void* threadM(void* arg);

void* threadGL(void* arg);

void* threadTGV(void* arg);

void exitTrain(int num);

void initTrain(const char* file);

void processTrain(int msqid, const char* file);

#endif