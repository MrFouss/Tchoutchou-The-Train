#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "communication.h"

typedef struct TrainEvent {
	int startTime; /*second*/
	TrainType type;
	Direction direction;
} TrainEvent;

typedef struct Parser {
	long currPosition;
	char file[50];
	int remEventNbr;
} Parser;

Parser initParser(const char*);

TrainEvent nextEvent(Parser*);

#endif