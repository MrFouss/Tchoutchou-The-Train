#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

typedef enum Direction {
	DIR_EW = 0,
	DIR_WE = 1
} Direction;

typedef enum TrainType {
    TYPE_TGV = 0,
	TYPE_GL = 1,
	TYPE_M = 2
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

typedef enum MessageType {
	MSG_REQUEST = 0,
	MSG_REQUEST_FORCE = 1,
	MSG_NOTIFICATION = 2,
	MSG_PERMISSION = 3
} MessageType;

typedef struct Message {
	long dst;
	long src;
	MessageType type;
	struct Train train;
} Message;


typedef struct MessageListElement {
    Message message;
    struct MessageListElement* next;
} MessageQueueElement;

typedef struct MessageQueue {
	MessageQueueElement* first;
	MessageQueueElement* last;
	int size;
} MessageQueue;

MessageQueue initQueue();

void offer(MessageQueue*, Message);

Message poll(MessageQueue*);

void removeQueue(MessageQueue*);

#endif