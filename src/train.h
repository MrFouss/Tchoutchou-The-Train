#ifndef TRAIN_H
#define TRAIN_H

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#include <unistd.h>
#endif

#include <signal.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <bits/sigthread.h>

#include "parser.h"
#include "header_master.h"

/**************/
/* PROTOTYPES */
/**************/

void trainHandlerSIGINT(int num);

void sendMessage(MessageType type, Message* msg, int dst, Train train);

/* Merchandise train thread */
void* threadMerchandise(void* arg);

/* Passenger (TGV or GL) train thread */
void* threadPassenger(void* arg);

/* Ensure that every allocated memory is properly freed */
void exitTrain(int num);

/* Initialize variables and launch train threads according to a file */
void initTrain(const char* file);

/* Launch a Train thread, from creation to termination */
void processTrain(const char* file);

#endif