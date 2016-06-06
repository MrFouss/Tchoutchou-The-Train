#include "train.h"

void* threadGrandeLigne(void* arg) {
	//TODO
	pthread_exit(NULL);
}

void* threadMarchandise(void* arg) {
	//TODO
	pthread_exit(NULL);
}

void* threadTGV(void* arg) {
	//TODO
	pthread_exit(NULL);
}

void initTrain(char* file) {
	//counter used to initialize a train with a unique id, 0 to 9 are reserved for processes and manager threads
	int idCounter = 10;

	//handle interruption signal
	signal(SIGINT, exitTrain);

	//TODO generate trains according to the specified file
}

void exitTrain() {
	//join threads

	exit(0);
}

void processTrain(int msqid, char* file)
{
	trainGlobalMsqid = msqid;
	initTrain(file);

	exitTrain();
}