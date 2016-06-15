#include "manager.h"

void handler(int num) {
    /* inform child process that they must finish their execution */
    pthread_kill(AIGUILLAGE1, SIGTERM);
    pthread_kill(AIGUILLAGE2, SIGTERM);
    pthread_kill(TUNNEL, SIGTERM);
    exitProgram();
}

void* managerThread(void* arg) {
	char name[30]="";
    long permissionFreq = ((ManagerThreadArg*)arg)->permissionFreq;
    int id = ((ManagerThreadArg*)arg)->id;
    Message msg;
    MessageQueue* mq = ((ManagerThreadArg*)arg)->messageQueue;
    int passingTrain = 0;

	strcpy(name, ((ManagerThreadArg*)arg)->name);

	printf("%s : My id is %d.\n", name, id);

    while(1) {
    	usleep(permissionFreq);
    	while (msgrcv(MANAGER_GLOBAL_MSQID, &msg, sizeof(Message) - sizeof(long), id, IPC_NOWAIT) != -1) {
	        switch (msg.type) {
	            case MSG_REQUEST_FORCE :
	                printf("%s : Recieved force request from train %li.\n", name, msg.src);
	                passingTrain++;
	                break;
	            case MSG_REQUEST :
	                printf("%s : Recieved request from train %li.\n", name, msg.src);
	                offer(mq, msg); /*store request in message queue*/
	                break;
	            case MSG_NOTIFICATION :
	            	printf("%s : Recieved notification from train %li.\n", name, msg.src);
	            	passingTrain--;
	                break;
                default :
                    fprintf(stderr, "Error: you're not supposed to go there");
	        }
    	}
    	if (passingTrain == 0 && mq->size != 0) { /*if there is no train ont the critical railway*/
    		/*TODO : manage a priority*/
    		passingTrain++;
    		msg = poll(mq);
    		msg.dst = msg.src;
    		msg.src = id;
    		msg.type = MSG_PERMISSION;
    		printf("%s : Informing train %li that it can pass.\n", name, msg.dst);
    		msgsnd(MANAGER_GLOBAL_MSQID, &msg, sizeof(Message) - sizeof(long), 0);
    	}
    }

	pthread_exit(NULL);
}

void exitManager(int num) {

    /* Join all 4 manager threads */
    int error;

    /*
    if(msgctl(MANAGER_MSQID, IPC_RMID, NULL) == -1) {
        fprintf(stderr, "Error while deleting the message queue.\n");
        fprintf(stderr, "\tError %d: %s\n", errno, strerror(errno));
        exit(-1);
    }
    */
    removeQueue(&A1MSG);
    removeQueue(&A2MSG);
    removeQueue(&TMSG);

    if((error = pthread_join(AIGUILLAGE1, NULL)) != 0) {
        fprintf(stderr, "Error while joining aiguillage1.\n");
        fprintf(stderr, "\tError %d: %s\n", error, strerror(error));
        exit(-1);
    }

    if((error = pthread_join(AIGUILLAGE2, NULL)) != 0) {
        fprintf(stderr, "Error while joining aiguillage2.\n");
        fprintf(stderr, "\tError %d: %s\n", error, strerror(error));
        exit(-1);
    }

    if((error = pthread_join(TUNNEL, NULL)) != 0) {
        fprintf(stderr, "Error while joining tunnel.\n");
        fprintf(stderr, "\tError %d: %s\n", error, strerror(error));
        exit(-1);
    }

    exit(0);
}

int initManager() {
    int error;
    pthread_attr_t thr_attr;

    /* handles interruption from SIGINT */
	signal(SIGINT, handler);

	/*
    if((MANAGER_MSQID = msgget(IPC_PRIVATE, IPC_CREAT | IPC_EXCL | 0640)) == -1) {
        fprintf(stderr, "Error while creating the message queue.\n");
        fprintf(stderr, "\tError %d: %s\n", errno, strerror(errno));
        return -1;
    }
    */

    /* Initialize the attributes of the threads */
    /*
    if(pthread_attr_init(&thr_attr) != 0) {
        fprintf(stderr, "Error while setting the attributes of the threads.\n");
        return -1;
    }
	*/

    /* Create all 4 manager threads */

    ManagerThreadArg a1, a2, t;

    strcpy(a1.name, "AIGUILLAGE 1");
    a1.id = 1;
    a1.permissionFreq = 500000;
    A1MSG = initQueue();
    a1.messageQueue = &A1MSG;

    if((error = pthread_create(&AIGUILLAGE1, NULL, managerThread, (void*)&a1)) != 0) {
        fprintf(stderr, "Error while creating aiguillage1.\n");
        fprintf(stderr, "\tError %d: %s\n", error, strerror(error));
        return -1;
    }

    strcpy(a2.name, "AIGUILLAGE 2");
    a2.id = 2;
    a2.permissionFreq = 500000;
    A1MSG = initQueue();
    a2.messageQueue = &A2MSG;

    if((error = pthread_create(&AIGUILLAGE2, NULL, managerThread, (void*)&a2)) != 0) {
        fprintf(stderr, "Error while creating aiguillage2.\n");
        fprintf(stderr, "\tError %d: %s\n", error, strerror(error));
        return -1;
    }

    strcpy(t.name, "TUNNEL");
    t.id = 3;
    t.permissionFreq = 500000;
    TMSG = initQueue();
    t.messageQueue = &TMSG;

    if((error = pthread_create(&TUNNEL, NULL, managerThread, (void*)&t)) != 0) {
        fprintf(stderr, "Error while creating tunnel.\n");
        fprintf(stderr, "\tError %d: %s\n", error, strerror(error));
        return -1;
	}

	sleep(2);

    return 0;
}

void processManager(int global_msqid) {

	MANAGER_GLOBAL_MSQID = global_msqid;

	if(initManager() == -1)
        fprintf(stderr, "Error during the initialization of the process manager.\n");

	exitManager(0);
}