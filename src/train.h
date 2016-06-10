#ifndef TRAIN_H
#define TRAIN_H

#include <pthread.h>
#include <signal.h>
#include <stdlib.h>

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
    POS_GARE = 0,
	POS_AIGUILLAGE = 1,
	POS_TUNNEL = 2,
	POS_VOIES = 3
} Position;

typedef struct Train {
	int id;
	TrainType type;
	Direction direction;
	int position;
} Train;

typedef struct TrainNode {
    pthread_t thread;
    struct TrainNode* next;
} TrainNode;

typedef struct TrainNode* TrainList;

/********************/
/* GLOBAL VARIABLES */
/********************/

TrainList TRAINS;
int TRAIN_GLOBAL_MSQID;

#endif