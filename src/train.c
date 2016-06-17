#include "train.h"

void trainHandlerSIGINT(int num) {
	int i;

	for (i = 0; i < TRAIN_NBR; i++) {
    	pthread_kill(TRAINS[i], SIGTERM);
    }
}

void sendMessage(MessageType type, Message* msg, int dst, Train train) {
	msg->dst = dst;
	msg->type = type;
	msg->train = train;
	msg->src = train.id;
	msgsnd(MSQID, msg, sizeof(Message) - sizeof(long), 0);
}

void* threadMerchandise(void* arg) {
	__useconds_t waitingTime = 1000000;
	Message msg;
	Train self;

	pthread_mutex_lock(&MUTEX_TRAINS);
	self = *(Train*)arg;
	pthread_cond_signal(&COND_TRAINS);
	pthread_mutex_unlock(&MUTEX_TRAINS);

	msg.src = self.id;

	printf("M %d: I am a M train.\n", self.id);

	if (self.direction == DIR_WE) {

		printf("M %d: Sending request to cross AIGUILLAGE 1.\n", self.id);
		sendMessage(MSG_REQUEST, &msg, ID_AIGUILLAGE_1, self);

		msgrcv(MSQID, &msg, sizeof(Message) - sizeof(long), self.id, 0);
		printf("M %d: Permission from AIGUILLAGE 1 received.\n", self.id);

		self.position = POS_VOIE_A;
		printf("M %d: Currently on VOIE A.\n", self.id);

        usleep(waitingTime);

		self.position = POS_AIGUILLAGE_1;
        printf("M %d: Currently on AIGUILLAGE 1.\n", self.id);

		usleep(waitingTime);

        self.position = POS_LIGNE_M_WE;
		printf("M %d: Currently on LIGNE M (W->E).\n", self.id);

        printf("M %d: Sending notification to AIGUILLAGE 1.\n", self.id);
		sendMessage(MSG_NOTIFICATION, &msg, ID_AIGUILLAGE_1, self);

        usleep(waitingTime);

        printf("M %d: Sending request to cross TUNNEL.\n", self.id);
		sendMessage(MSG_REQUEST, &msg, ID_TUNNEL, self);

        msgrcv(MSQID, &msg, sizeof(Message) - sizeof(long), self.id, 0);
		printf("M %d: Permission from TUNNEL received.\n", self.id);

		pthread_mutex_lock(&MUTEX_TUNNEL);

		self.position = POS_TUNNEL;
        printf("M %d: Currently in TUNNEL.\n", self.id);

		usleep(waitingTime);

		pthread_mutex_unlock(&MUTEX_TUNNEL);

		self.position = POS_LIGNE;
        printf("M %d: Currently on LIGNE.\n", self.id);

		usleep(waitingTime);

		printf("M %d: Sending notification to TUNNEL.\n", self.id);
		sendMessage(MSG_NOTIFICATION, &msg, ID_TUNNEL, self);

		printf("M %d: W->E traversal over. Success!\n", self.id);

	} else {

		printf("M %d: Sending request to access LIGNE and cross TUNNEL.\n", self.id);
		sendMessage(MSG_REQUEST, &msg, ID_TUNNEL, self);

		msgrcv(MSQID, &msg, sizeof(Message) - sizeof(long), self.id, 0);
		printf("M %d: Permission from TUNNEL received.\n", self.id);

		self.position = POS_LIGNE;
		printf("M %d: Currently on LIGNE.\n", self.id);

        usleep(waitingTime);

		pthread_mutex_lock(&MUTEX_TUNNEL);

		self.position = POS_TUNNEL;
        printf("M %d: Currently in TUNNEL.\n", self.id);

		usleep(waitingTime);

		pthread_mutex_unlock(&MUTEX_TUNNEL);

        self.position = POS_LIGNE_M_EW;
		printf("M %d: Currently on LIGNE M (E->W).\n", self.id);

		printf("M %d: Sending notification to TUNNEL.\n", self.id);
		sendMessage(MSG_NOTIFICATION, &msg, ID_TUNNEL, self);
		
		usleep(waitingTime);

        printf("M %d: Sending request to cross AIGUILLAGE 1.\n", self.id);
		sendMessage(MSG_REQUEST, &msg, ID_AIGUILLAGE_1, self);


        msgrcv(MSQID, &msg, sizeof(Message) - sizeof(long), self.id, 0);
		printf("M %d: Permission from AIGUILLAGE 1 received.\n", self.id);

		self.position = POS_AIGUILLAGE_1;
        printf("M %d: Currently on AIGUILLAGE 1.\n", self.id);

		usleep(waitingTime);

		self.position = POS_VOIE_B;
        printf("M %d: Currently on VOIE B.\n", self.id);

		printf("M %d: Sending notification to AIGUILLAGE 1.\n", self.id);
		sendMessage(MSG_NOTIFICATION, &msg, ID_AIGUILLAGE_1, self);

		usleep(waitingTime);

		printf("M %d: E->W traversal over. Success!\n", self.id);
    }

	pthread_exit(NULL);
}

void* threadPassenger(void* arg) {
	__useconds_t waitingTime;
	char* typeString;
	Message msg;
	Train self;

	pthread_mutex_lock(&MUTEX_TRAINS);
	self = *(Train*)arg;
	pthread_cond_signal(&COND_TRAINS);
	pthread_mutex_unlock(&MUTEX_TRAINS);

	msg.src = self.id;

	if (self.type == TYPE_TGV) {
		typeString = "TGV";
		waitingTime = 1000000;
	}
	else {
		typeString = "GL";
		waitingTime = 1000000;
	}

	printf("%s %d : I am a %s train.\n", typeString, self.id, typeString);

	if (self.direction == DIR_WE) {

		self.position = POS_VOIE_C;
		printf("%s %d: Currently on VOIE C.\n", typeString, self.id);

		printf("%s %d: Waiting at the station.\n", typeString, self.id);
		usleep(5000000);

		printf("%s %d: Sending request to cross AIGUILLAGE 2.\n", typeString, self.id);
		sendMessage(MSG_REQUEST, &msg, ID_AIGUILLAGE_2, self);

		msgrcv(MSQID, &msg, sizeof(Message) - sizeof(long), self.id, 0);
		printf("%s %d: Permission from AIGUILLAGE 2 received.\n", typeString, self.id);

		usleep(waitingTime);

		self.position = POS_AIGUILLAGE_2;
		printf("%s %d: Currently on AIGUILLAGE 2.\n", typeString, self.id);

		usleep(waitingTime);

		if(self.type == TYPE_TGV) {
			self.position = POS_LIGNE_TGV;
			printf("%s %d: Currently on LIGNE TGV.\n", typeString, self.id);
		} else {
			self.position = POS_LIGNE_GL;
			printf("%s %d: Currently on LIGNE GL.\n", typeString, self.id);
		}

		printf("%s %d: Sending notification to AIGUILLAGE 2.\n", typeString, self.id);
		sendMessage(MSG_NOTIFICATION, &msg, ID_AIGUILLAGE_2, self);

		usleep(waitingTime);

		printf("%s %d: Sending request to cross TUNNEL.\n", typeString, self.id);
		sendMessage(MSG_REQUEST, &msg, ID_TUNNEL, self);

		msgrcv(MSQID, &msg, sizeof(Message) - sizeof(long), self.id, 0);
		printf("%s %d: Permission from TUNNEL received.\n", typeString, self.id);

		pthread_mutex_lock(&MUTEX_TUNNEL);

		self.position = POS_TUNNEL;
		printf("%s %d: Currently in TUNNEL.\n", typeString, self.id);

		usleep(waitingTime);

		pthread_mutex_unlock(&MUTEX_TUNNEL);

		self.position = POS_LIGNE;
		printf("%s %d: Currently on LIGNE.\n", typeString, self.id);

		usleep(waitingTime);

		printf("%s %d: Sending notification to TUNNEL.\n", typeString, self.id);
		sendMessage(MSG_NOTIFICATION, &msg, ID_TUNNEL, self);

		printf("%s %d: W->E traversal over. Success!\n", typeString, self.id);

	} else {

		printf("%s %d: Sending request to access LIGNE and cross TUNNEL.\n", typeString, self.id);
		sendMessage(MSG_REQUEST, &msg, ID_TUNNEL, self);

		msgrcv(MSQID, &msg, sizeof(Message) - sizeof(long), self.id, 0);
		printf("%s %d: Permission from TUNNEL received.\n", typeString, self.id);

		self.position = POS_LIGNE;
		printf("%s %d: Currently on LIGNE.\n", typeString, self.id);

		usleep(waitingTime);

		pthread_mutex_lock(&MUTEX_TUNNEL);

		self.position = POS_TUNNEL;
		printf("%s %d: Currently in TUNNEL.\n", typeString, self.id);

		usleep(waitingTime);

		pthread_mutex_unlock(&MUTEX_TUNNEL);

		if(self.type == TYPE_TGV) {
			self.position = POS_LIGNE_TGV;
			printf("%s %d: Currently on LIGNE TGV.\n", typeString, self.id);
		} else {
			self.position = POS_LIGNE_GL;
			printf("%s %d: Currently on LIGNE GL.\n", typeString, self.id);
		}

		printf("%s %d: Sending notification to TUNNEL.\n", typeString, self.id);
		sendMessage(MSG_NOTIFICATION, &msg, ID_TUNNEL, self);

		usleep(waitingTime);

		printf("%s %d: Sending request to cross AIGUILLAGE 2.\n", typeString, self.id);
		sendMessage(MSG_REQUEST, &msg, ID_AIGUILLAGE_2, self);

		msgrcv(MSQID, &msg, sizeof(Message) - sizeof(long), self.id, 0);
		printf("%s %d: Permission from AIGUILLAGE 2 received.\n", typeString, self.id);

		self.position = POS_AIGUILLAGE_2;
		printf("%s %d: Currently on AIGUILLAGE 2.\n", typeString, self.id);

		usleep(waitingTime);

		self.position = POS_VOIE_D;
		printf("%s %d: Currently on VOIE D.\n", typeString, self.id);

		printf("%s %d: Sending notification to AIGUILLAGE 2.\n", typeString, self.id);
		sendMessage(MSG_NOTIFICATION, &msg, ID_AIGUILLAGE_2, self);

		printf("%s %d: Waiting at the station.\n", typeString, self.id);
		usleep(5000000);

		printf("%s %d: E->W traversal over. Success!\n", typeString, self.id);
	}

	pthread_exit(NULL);
}

void exitTrain(int num) {
    int i;

    for (i = 0; i < TRAIN_NBR; i++) {
    	pthread_join(TRAINS[i], NULL);
    }

    /* free the allocation */
    free(TRAINS);

    exit(0);
}

void initTrain(const char* file) {
	time_t start = time(NULL);
	/* counter used to initialize a train with a unique id, 0 to 9 are reserved for processes and manager threads */
	int idCounter = 10;
	int error;
	Parser p;
	TrainEvent te;
	Train t;
	pthread_t tr;

	pthread_mutex_init(&MUTEX_TRAINS, NULL);
	pthread_mutex_init(&MUTEX_TUNNEL, NULL);

	/* handle interruption signal SIGINT */
	signal(SIGINT, trainHandlerSIGINT);

	/* init parser */
    p = initParser(file);

    TRAINS = (pthread_t*)malloc(sizeof(pthread_t)*p.remEventNbr);
	TRAIN_NBR = 0;

    /* while there is remaining events */
    while (p.remEventNbr > 0) {
	    
	    /* get next event */
	    te = nextEvent(&p);

	    /* wait for the beginning of the event */
	    usleep((__useconds_t) (1000000*(te.startTime - (time(NULL) - start))));

		pthread_mutex_lock(&MUTEX_TRAINS);
	    /* set train thread argument */
		t.id = idCounter++;
		t.type = te.type;
		t.direction = te.direction;

		/* launch thread */
		if(t.type == TYPE_M) {
			if((error = pthread_create(&tr, NULL, threadMerchandise, (void*)&t)) != 0) {
				fprintf(stderr, "Error while creating M train thread.\n");
				fprintf(stderr, "\tError %d: %s\n", error, strerror(error));
			} else {
				TRAINS[TRAIN_NBR] = tr;
				TRAIN_NBR++;
				pthread_cond_wait(&COND_TRAINS, &MUTEX_TRAINS);
			}
		} else {
			if((error = pthread_create(&tr, NULL, threadPassenger, (void*)&t)) != 0) {
				fprintf(stderr, "Error while creating passenger train thread.\n");
				fprintf(stderr, "\tError %d: %s\n", error, strerror(error));
			} else {
				TRAINS[TRAIN_NBR] = tr;
				TRAIN_NBR++;
				pthread_cond_wait(&COND_TRAINS, &MUTEX_TRAINS);
			}
		}
		pthread_mutex_unlock(&MUTEX_TRAINS);
	}
}

void processTrain(const char* file) {
	initTrain(file);

	exitTrain(0);
}