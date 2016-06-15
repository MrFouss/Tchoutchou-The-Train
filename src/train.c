#include "train.h"

void trainHandlerSIGINT(int num) {
	int i;

	for (i = 0; i < TRAIN_NBR; i++) {
    	pthread_kill(TRAINS[i], SIGTERM);
    }
}

void* threadM(void* arg) {
	Train self = *(Train*)arg;
	Message msg;
	msg.src = self.id;

	printf("%d : I am a M train.\n", self.id);

	if (self.direction == DIR_WE) {

		self.position = POS_VOIEA;

		printf("%d : Informing AIGUILLAGE 1 of my arrival\n", self.id);
		msg.dst = 1;
		msg.type = MSG_REQUEST_FORCE;
		msg.train = self;
		msgsnd(MSQID, &msg, sizeof(Message) - sizeof(long), 0);

        usleep(1000000);

        printf("%d : Arriving at AIGUILLAGE 1\n", self.id);
		self.position = POS_AIGUILLAGE1;

		usleep(1000000);

		self.position = POS_LIGNEM;
        printf("%d : Informing AIGUILLAGE 1 that I passed AIGUILLAGE 1\n", self.id);
		msg.dst = 1;
		msg.type = MSG_NOTIFICATION;
		msg.train = self;
		msgsnd(MSQID, &msg, sizeof(Message) - sizeof(long), 0);

        usleep(1000000);

        printf("%d : Informing TUNNEL that I would like to access TUNNEL (waiting for authorization)\n", self.id);
		msg.dst = 3;
		msg.type = MSG_REQUEST;
		msg.train = self;
		msgsnd(MSQID, &msg, sizeof(Message) - sizeof(long), 0);

        msgrcv(MSQID, &msg, sizeof(Message) - sizeof(long), self.id, 0);
		printf("%d : received authorization from TUNNEL to pass the TUNNEL\n", self.id);
		self.position = POS_TUNNEL;

		usleep(1000000);

		self.position = POS_LIGNE;
		printf("%d : Inform TUNNEL I passed the TUNNEL\n", self.id);
		msg.train = self;
		msg.dst = 3;
		msg.type = MSG_NOTIFICATION;
		msgsnd(MSQID, &msg, sizeof(Message) - sizeof(long), 0);

	} else if (self.direction == DIR_WE) {

    }

	pthread_exit(NULL);
}

void* threadGL(void* arg) {
	Train self = *(Train*)arg;
	Message msg;
	msg.src = self.id;

	printf("%d : I am a GL train.\n", self.id);

	if (self.direction == DIR_WE) {

		self.position = POS_VOIEC;

        usleep(3000000);

		printf("%d : Informing AIGUILLAGE 2 that I would like to access it (waiting for authorization)\n", self.id);
		msg.dst = 1;
		msg.type = MSG_REQUEST;
		msg.train = self;
		msgsnd(MSQID, &msg, sizeof(Message) - sizeof(long), 0);

        msgrcv(MSQID, &msg, sizeof(Message) - sizeof(long), self.id, 0);
        printf("%d : received authorization from AIGUILLAGE 2 to access it\n", self.id);

		usleep(1000000);

		printf("%d : Arriving at AIGUILLAGE 2\n", self.id);
        self.position = POS_AIGUILLAGE2;

		usleep(1000000);

		self.position = POS_LIGNEGL;
		printf("%d : Informing AIGUILLAGE 2 that I passed it\n", self.id);
		msg.dst = 1;
		msg.type = MSG_NOTIFICATION;
		msg.train = self;
		msgsnd(MSQID, &msg, sizeof(Message) - sizeof(long), 0);

		usleep(1000000);

		printf("%d : Informing TUNNEL that I would like to access it (waiting for authorization)\n", self.id);
		msg.dst = 3;
		msg.type = MSG_REQUEST;
		msg.train = self;
		msgsnd(MSQID, &msg, sizeof(Message) - sizeof(long), 0);

		msgrcv(MSQID, &msg, sizeof(Message) - sizeof(long), self.id, 0);
		printf("%d : received authorization from TUNNEL to pass the TUNNEL\n", self.id);
		self.position = POS_TUNNEL;

		usleep(1000000);

		self.position = POS_LIGNE;
		printf("%d : Inform TUNNEL I passed the TUNNEL\n", self.id);
		msg.train = self;
		msg.dst = 3;
		msg.type = MSG_NOTIFICATION;
		msgsnd(MSQID, &msg, sizeof(Message) - sizeof(long), 0);

	} else if (self.direction == DIR_WE) {

	}

	pthread_exit(NULL);
}

void* threadTGV(void* arg) {
    Train self = *(Train*)arg;
    Message msg;
    msg.src = self.id;

    printf("%d : I am a TGV train.\n", self.id);

    if (self.direction == DIR_WE) {

        self.position = POS_VOIEC;

        usleep(5000000);

        printf("%d : Informing AIGUILLAGE 2 that I would like to access it (waiting for authorization)\n", self.id);
        msg.dst = 1;
        msg.type = MSG_REQUEST;
        msg.train = self;
        msgsnd(MSQID, &msg, sizeof(Message) - sizeof(long), 0);

        msgrcv(MSQID, &msg, sizeof(Message) - sizeof(long), self.id, 0);
        printf("%d : received authorization from AIGUILLAGE 2 to access it\n", self.id);

        usleep(800000);

        printf("%d : Arriving at AIGUILLAGE 2\n", self.id);
        self.position = POS_AIGUILLAGE2;

        usleep(800000);

        self.position = POS_LIGNETGV;
        printf("%d : Informing AIGUILLAGE 2 that I passed it\n", self.id);
        msg.dst = 1;
        msg.type = MSG_NOTIFICATION;
        msg.train = self;
        msgsnd(MSQID, &msg, sizeof(Message) - sizeof(long), 0);

        usleep(800000);

        printf("%d : Informing TUNNEL that I would like to access it (waiting for authorization)\n", self.id);
        msg.dst = 3;
        msg.type = MSG_REQUEST;
        msg.train = self;
        msgsnd(MSQID, &msg, sizeof(Message) - sizeof(long), 0);

        msgrcv(MSQID, &msg, sizeof(Message) - sizeof(long), self.id, 0);
        printf("%d : received authorization from TUNNEL to pass the TUNNEL\n", self.id);
        self.position = POS_TUNNEL;

        usleep(800000);

        self.position = POS_LIGNE;
        printf("%d : Inform TUNNEL I passed the TUNNEL\n", self.id);
        msg.train = self;
        msg.dst = 3;
        msg.type = MSG_NOTIFICATION;
        msgsnd(MSQID, &msg, sizeof(Message) - sizeof(long), 0);

    } else if (self.direction == DIR_WE) {

    }

    pthread_exit(NULL);
}

void exitTrain(int num) {
	/* Join all 4 manager threads */
    int i;

    for (i = 0; i < TRAIN_NBR; i++) {
    	pthread_join(TRAINS[i], NULL);
    }

    /* free allocation */
    free(TRAINS);

    exit(0);
}

void initTrain(const char* file) {
	time_t start = time(NULL);
	/* Counter used to initialize a train with a unique id, 0 to 9 are reserved for processes and manager threads */
	int idCounter = 10;
	int error;
	TrainEvent te;
	Train t;
	pthread_t tr;

	TRAIN_NBR = 0;
	TRAINS = (pthread_t*)malloc(sizeof(pthread_t)*10);

	/* Handle interruption signal SIGINT */
	signal(SIGINT, trainHandlerSIGINT);

	/* init parser */
    Parser p = initParser(file);

    /* while there is remaining vents */
    while (p.remEventNbr > 0) {
	    
	    /* get next event */
	    te = nextEvent(&p);

	    /* wait for the beginning of the event */
	    usleep(1000000*(te.startTime - (time(NULL) - start)));

	    /* set train thread argument */
		t.id = idCounter++;
		t.type = te.type;
		t.direction = te.direction;

		/* lauc thread */
		switch (t.type) {
			case TYPE_M :
				if((error = pthread_create(&tr, NULL, threadM, (void*)&t)) != 0) {
					fprintf(stderr, "Error while creating M train thread.\n");
					fprintf(stderr, "\tError %d: %s\n", error, strerror(error));
				} else {
					TRAINS[TRAIN_NBR] = tr;
					TRAIN_NBR++;
				}
				break;
            case TYPE_GL :
                if((error = pthread_create(&tr, NULL, threadGL, (void*)&t)) != 0) {
                    fprintf(stderr, "Error while creating GL train thread.\n");
                    fprintf(stderr, "\tError %d: %s\n", error, strerror(error));
                } else {
                    TRAINS[TRAIN_NBR] = tr;
                    TRAIN_NBR++;
                }
                break;
            case TYPE_TGV :
                if((error = pthread_create(&tr, NULL, threadTGV, (void*)&t)) != 0) {
                    fprintf(stderr, "Error while creating TGV train thread.\n");
                    fprintf(stderr, "\tError %d: %s\n", error, strerror(error));
                } else {
                    TRAINS[TRAIN_NBR] = tr;
                    TRAIN_NBR++;
                }
                break;
            default :
                break;
		}

		/* wait 1 second to be sure the thread had time to copy its argument */
		usleep(1000000);
	}
}

void processTrain(int msqid, const char* file) {
	initTrain(file);

	exitTrain(0);
}