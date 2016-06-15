#ifndef TRAIN_H
#define TRAIN_H

#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/msg.h>
#include <bits/sigthread.h>

#include "communication.h"
#include "parser.h"

/**************/
/* PROTOTYPES */
/**************/

/** 
 * Merchandise train thread
 */
void* threadMerchandise(void* arg);

/** 
 * Passenger (TGV or GL) train thread
 */
void* threadPassenger(void* arg);

/** 
 * ensure that every allocated memory is properly freed
 */
void exitTrain(int num);

/** 
 * Initialize variables and launch train threads according to a file
 */
void initTrain(const char* file);

/** 
 * Launches a Train thread, from creation to termination
 */
void processTrain(int msqid, const char* file);

#endif