#ifndef TRAIN_H
#define TRAIN_H

typedef enum Direction {
	EW,
	WE
} Direction;

typedef enum TrainType {
	M,
	GL,
	TGV
} TrainType;

typedef struct Train {
	int id;
	TrainType type;
	Direction direction;
	int position;
} Train;

pthread_t* trains;
int trainGlobalMsqid;

#endif