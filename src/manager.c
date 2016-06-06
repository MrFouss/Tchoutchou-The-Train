#include "manager.h"

void* threadP0(void* arg) {
	//TODO
	pthread_exit(NULL);
}

void* threadP1(void* arg) {
	//TODO
	pthread_exit(NULL);
}

void* threadP2(void* arg) {
	//TODO
	pthread_exit(NULL);
}

void* threadP3(void* arg) {
	//TODO
	pthread_exit(NULL);
}

void initManager() {
	//handle interruption signal
	signal(SIGINT, exitManager);

	//TODO build manager message queue
	//TODO launch threads
}

void exitManager() {
	//TODO join manager threads, message queue, ...
	exit(0);
}

void processManager(int msqid)
{
	//inter-process msqid
	managerGlobalMsqid = msqid;

	initManager();

	exitManager();
}