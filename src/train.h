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

typedef enum Direction {
	DIR_EW,
	DIR_WE
} Direction;

typedef enum TrainType {
    TYPE_TGV = 1,
	TYPE_GL = 2,
	TYPE_M = 3
} TrainType;

typedef enum Position {
    POS_VOIEC = 0,
    POS_VOIED = 1,
	POS_AIGUILLAGE1 = 2,
	POS_AIGUILLAGE2 = 3,
	POS_TUNNEL = 4,
	POS_LIGNE = 5,
	POS_VOIEA = 6,
	POS_VOIEB = 7,
	POS_LIGNETGV = 8,
	POS_LIGNEM = 9,
	POS_LIGNEGL = 10
} Position;

typedef struct Train {
	int id;
	TrainType type;
	Direction direction;
	Position position;
} Train;

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