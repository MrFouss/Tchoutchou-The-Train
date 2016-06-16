#ifndef MANAGER_H
#define MANAGER_H

#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "communication.h"
#include "main.h"

/**************/
/* PROTOTYPES */
/**************/

/* Called to interrupt process (signal handler) */
void managerHandlerSIGINT(int num);

/* Critical zone manager thread function */
void* managerThread(void* arg);

/* Called to free allocations and properly terminate the process */
void exitManager(int num);

/* Initialize the manager variables and launch manager threads */
int initManager();

/* Function called to initialize a manager thread and terminate it eventually */
void processManager();

/* Check if the train can cross AIGUILLAGE 1 or AIGUILLAGE 2 */
bool canPassAiguillage(Train train);

/* Check if the train can cross TUNNEL and LIGNE */
bool canPassTunnel(Train train);

void resolveRequest(char* name, MessageList* ml);

void resolveNotification(Message msg);

void sendPermission(MessageType type, Message* msg);

#endif
