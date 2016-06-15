#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

/* train related structures */

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

/* message queue messages structures (sent through message queue) */

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

/* message list structures */

typedef struct MessageListElement {
    Message message;
    struct MessageListElement* next;
} MessageListElement;

typedef struct MessageList {
	MessageListElement* first;
	MessageListElement* last;
	int size;
} MessageList;

/********************/
/* GLOBAL VARIABLES */
/********************/

/* Parent process */
pid_t PROGRAM;

/* Children process PID */
pid_t MANAGER, TRAIN;

/* Inter-process message queue */
int MSQID;

/* manager threads, 1 per critial zone */
pthread_t AIGUILLAGE1, AIGUILLAGE2, TUNNEL;

/* list of messages used by manager threads to store messages */
MessageList A1MSG, A2MSG, TMSG;

/* pending non read messages */
int PENDING_MSG;

/* list of thrains */
pthread_t* TRAINS;

/* number of trains */
int TRAIN_NBR;

/**************/
/* PROTOTYPES */
/**************/

/**
 * Initialize an empty message list
 */
MessageList initList();

/**
 * Add a message at the end of the list
 */
void offer(MessageList*, Message);

/**
 * Pop and return the first message of the list
 */
Message poll(MessageList*);

/**
 * Remove every messages from the list, thus freeing previously allocated memory
 */
void removeList(MessageList*);

#endif