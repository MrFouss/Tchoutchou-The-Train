#include "train.h"

void* threadGL(void* arg) {
	// TODO
	pthread_exit(NULL);
}

void* threadM(void* arg) {
	// TODO
	pthread_exit(NULL);
}

void* threadTGV(void* arg) {
	// TODO
	pthread_exit(NULL);
}

void exitTrain() {
	//join threads

	exit(0);
}

void initTrain(char* file) {
	// Counter used to initialize a train with a unique id, 0 to 9 are reserved for processes and manager threads
	int idCounter = 10;

	// Handle interruption signal SIGINT
	signal(SIGINT, exitTrain);

	//TODO generate trains according to the specified file
}

void processTrain(int msqid, char* file) {
	TRAIN_GLOBAL_MSQID = msqid;
	initTrain(file);

	exitTrain();
}