#ifndef TRAIN_H
#define TRAIN_H

#include <pthread.h>
#include <signal.h>

typedef enum Direction {
	EW,
	WE
} Direction;

typedef enum TrainType {
    TGV = 1,
	GL = 2,
    M = 3
} TrainType;

typedef enum Position {
    GARE = 0,
    AIGUILLAGE = 1,
    TUNNEL = 2,
    VOIES = 3
} Position;

typedef struct Train {
	int id;
	TrainType type;
	Direction direction;
	int position;
} Train;

pthread_t* trains;
int trainGlobalMsqid;

#endif