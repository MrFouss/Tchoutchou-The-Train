#ifndef TRAIN_H
#define TRAIN_H

#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "communication.h"

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

void exitTrain(int num);

void initTrain(const char* file);

void processTrain(int msqid, const char* file);

#endif