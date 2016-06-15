#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "communication.h"

/* structure describing a train arrival in the system */

typedef struct TrainEvent {
	int startTime; /* in microseconds */
	TrainType type;
	Direction direction;
} TrainEvent;

/* describes the stat of reading of a train event file */

typedef struct Parser {
	long currPosition;
	char file[50];
	int remEventNbr;
} Parser;

/**
 * Initialize a Parser for a specific file. The cursor is position at the beginning of the first event
 */
Parser initParser(const char*);

/**
 * return the next upcomming train event and advance the cursor to the next one
 */
TrainEvent nextEvent(Parser*);

#endif