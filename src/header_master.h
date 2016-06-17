#ifndef HEADER_MASTER_H
#define HEADER_MASTER_H

#include <signal.h>
#include <pthread.h>

/**********/
/* MACROS */
/**********/

#define ID_AIGUILLAGE_1 1
#define ID_AIGUILLAGE_2 2
#define ID_TUNNEL 3

/*************************/
/* STRUCTURES DEFINITION */
/*************************/

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
    POS_VOIE_A = 0,
    POS_VOIE_B = 1,
    POS_VOIE_C = 2,
    POS_VOIE_D = 3,
    POS_AIGUILLAGE_1 = 4,
    POS_AIGUILLAGE_2 = 5,
    POS_LIGNE_TGV = 6,
    POS_LIGNE_M_WE = 7,
    POS_LIGNE_M_EW = 8,
    POS_LIGNE_GL = 9,
    POS_TUNNEL = 10,
    POS_LIGNE = 11
} Position;

typedef struct Train {
    int id;
    TrainType type;
    Direction direction;
    Position position;
} Train;

/* Message queue messages structures (sent through message queue) */

typedef enum MessageType {
    MSG_REQUEST = 0,
    MSG_NOTIFICATION = 1,
    MSG_PERMISSION = 2
} MessageType;

typedef struct Message {
    long dst;
    long src;
    MessageType type;
    Train train;
} Message;

/* Message list structures */

typedef struct MessageListElement {
    Message message;
    struct MessageListElement* next;
} MessageListElement;

typedef struct MessageList {
    MessageListElement* first;
    MessageListElement* last;
    int size;
} MessageList;

typedef struct ManagerThreadArg {
    char name[30];
    int id;
    long permissionFreq; /* in microseconds */
    MessageList* messageList;
} ManagerThreadArg;

/* Structure describing a train arrival in the system */
typedef struct TrainEvent {
    int startTime; /* in microseconds */
    TrainType type;
    Direction direction;
} TrainEvent;

/* Describe the reading status of a train event file */
typedef struct Parser {
    long currPosition;
    char file[50];
    int remEventNbr;
} Parser;

/********************/
/* GLOBAL VARIABLES */
/********************/

/* Children process PID */
pid_t MANAGER, TRAIN;

/* Inter-process message queue */
int MSQID;

/* Manager threads, one per critical zone */
pthread_t AIGUILLAGE1, AIGUILLAGE2, TUNNEL;

/* List of messages used by manager threads to store messages */
MessageList A1MSG, A2MSG, TMSG;

/**
 * Counters for the amount of trains on unidirectional tracks
 *
 * if X > 0 : there are X trains going west-east on the line
 * if X < 0 : there are -X trains going east-west on the line
 * if X = 0 : there are no trains on the line
 */
int CMP_LIGNE_TGV, CMP_LIGNE_GL, CMP_TUNNEL, CMP_AIGUILLAGE_1, CMP_AIGUILLAGE_2;

/* Mutex used to lock and protect the counters */
pthread_mutex_t MUTEX_COUNT;

/* Mutex used to create safely the trains */
pthread_mutex_t MUTEX_TRAINS;

/* Condition on the use of the arguments of the trains */
pthread_cond_t COND_TRAINS;

/* List of trains */
pthread_t* TRAINS;

/* Total number of trains */
int TRAIN_NBR;

#endif
