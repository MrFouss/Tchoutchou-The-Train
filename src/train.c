#include "train.h"

void SIGINThandle(int num) {
	int i;

	for (i = 0; i < TRAIN_NBR; i++) {
    	pthread_kill(TRAINS[i], SIGTERM);
    }
}

void hand(int num) {
	return;	
}

void* threadM(void* arg) {
	Train self = *(Train*)arg;
	Message msg;
	msg.src = self.id;
	printf("%d : I am %d\n", self.id, self.id);
	msg.train = &self;

	if (self.direction == DIR_WE) {
		self.position = POS_VOIEA;
		printf("%d : informing P1 of my arrival\n", self.id);
		msg.dst = 1;
		msg.type = MSG_REQUEST_FORCE;
		msgsnd(TRAIN_GLOBAL_MSQID, &msg, sizeof(Message) - sizeof(long), 0);
		usleep(10000);
		printf("%d : arriving at aiguillage 1\n", self.id);

		self.position = POS_AIGUILLAGE1;
		usleep(10000);

		self.position = POS_LIGNEM;
		printf("%d : informing P1 that I passed aiguillage 1\n", self.id);
		msg.dst = 1;
		msg.type = MSG_ACKNOWLEDGE;
		msgsnd(TRAIN_GLOBAL_MSQID, &msg, sizeof(Message) - sizeof(long), 0);
		usleep(10000);
		printf("%d : informing P2 that I would like to access tunnel (waiting)\n", self.id);
		msg.dst = 2;
		msg.type = MSG_REQUEST;
		msgsnd(TRAIN_GLOBAL_MSQID, &msg, sizeof(Message) - sizeof(long), 0);
		msgrcv(TRAIN_GLOBAL_MSQID, &msg, sizeof(Message) - sizeof(long), self.id, 0);
		printf("%d : recieved authorization from P2 to pass the tunnel\n", self.id);

		self.position = POS_TUNNEL;
		usleep(10000);

		self.position = POS_LIGNE;
		printf("%d : inform P2 I passed the tunnel\n", self.id);
		msg.dst = 2;
		msg.type = MSG_ACKNOWLEDGE;
		msgsnd(TRAIN_GLOBAL_MSQID, &msg, sizeof(Message) - sizeof(long), 0);
	}

	pthread_exit(NULL);
}

void exitTrain(int num) {
	/* Join all 4 manager threads */
    int error, i;

    for (i = 0; i < TRAIN_NBR; i++) {
    	pthread_join(TRAINS[i], NULL);
    }

    free(TRAINS);

    exit(0);
}

void initTrain(const char* file) {
	/* Counter used to initialize a train with a unique id, 0 to 9 are reserved for processes and manager threads */
	int idCounter = 10;
	int error;

	TRAIN_NBR = 0;
	TRAINS = (pthread_t*)malloc(sizeof(pthread_t)*10);

	/* Handle interruption signal SIGINT */
	signal(SIGINT, SIGINThandle);
	signal(SIGCONT, hand);

	Train t;
	t.id = 11;
	t.type = TYPE_M;
	t.direction = DIR_WE;
	t.position = POS_VOIEA;


	printf("creating thread train !\n");
	pthread_t tr;
	if((error = pthread_create(&tr, NULL, threadM, (void*)&t)) != 0) {
        fprintf(stderr, "Error while creating train thread.\n");
        fprintf(stderr, "\tError %d: %s\n", error, strerror(error));
    } else {
    	TRAINS[TRAIN_NBR] = tr;
    	TRAIN_NBR++;
    }
}

void processTrain(int msqid, const char* file) {
	TRAIN_GLOBAL_MSQID = msqid;
	initTrain(file);

	exitTrain(0);
}