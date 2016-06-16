#ifndef PARSER_H
#define PARSER_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "header_master.h"

/**************/
/* PROTOTYPES */
/**************/

/* Initialize a Parser for a specific file. The cursor is position at the beginning of the first event */
Parser initParser(const char*);

/* Return the next upcoming train event and advance the cursor to the next one */
TrainEvent nextEvent(Parser*);

#endif