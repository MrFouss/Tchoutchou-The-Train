#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <stdlib.h>
#include <stdio.h>
#include "train.h"
#include <pthread.h>

typedef enum MessageType {
	MSG_REQUEST = 0,
	MSG_REQUEST_FORCE = 1,
	MSG_ACKNOWLEDGE = 2
} MessageType;

typedef struct Message {
	long dst;
	long src;
	MessageType type;
	Train* train;
} Message;

#endif