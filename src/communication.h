#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <stdlib.h>

#include "main.h"

/**************/
/* PROTOTYPES */
/**************/

/* Initialize an empty message list */
struct MessageList initList();

/* Add a message at the end of the list */
void offer(MessageList*, Message);

/* Pop and return the first message of the list */
Message poll(MessageList*);

/* Remove every messages from the list, thus freeing previously allocated memory */
void removeList(MessageList*);

#endif