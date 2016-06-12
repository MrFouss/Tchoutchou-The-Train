#include "manager.h"

void* threadP0(void* arg) {
	/* TODO */
	pthread_exit(NULL);
}

void* threadP1(void* arg) {
	/* TODO */
	pthread_exit(NULL);
}

void* threadP2(void* arg) {
	/* TODO */
	pthread_exit(NULL);
}

void* threadP3(void* arg) {
	/* TODO */
	pthread_exit(NULL);
}

void exitManager(int num) {

    /* Join all 4 manager threads */
    int error;

    if(msgctl(MANAGER_MSQID, IPC_RMID, NULL) == -1) {
        fprintf(stderr, "Error while deleting the message queue.\n");
        fprintf(stderr, "\tError %d: %s\n", errno, strerror(errno));
        exit(-1);
    }

    if((error = pthread_join(P0, NULL)) != 0) {
        fprintf(stderr, "Error while joining P0.\n");
        fprintf(stderr, "\tError %d: %s\n", error, strerror(error));
        exit(-1);
    }

    if((error = pthread_join(P1, NULL)) != 0) {
        fprintf(stderr, "Error while joining P1.\n");
        fprintf(stderr, "\tError %d: %s\n", error, strerror(error));
        exit(-1);
    }

    if((error = pthread_join(P2, NULL)) != 0) {
        fprintf(stderr, "Error while joining P2.\n");
        fprintf(stderr, "\tError %d: %s\n", error, strerror(error));
        exit(-1);
    }

    if((error = pthread_join(P3, NULL)) != 0) {
        fprintf(stderr, "Error while joining P3.\n");
        fprintf(stderr, "\tError %d: %s\n", error, strerror(error));
        exit(-1);
    }

    exit(0);
}

int initManager() {
    int error;
    pthread_attr_t thr_attr;

    /* handles interruption from SIGINT */
	signal(SIGINT, exitManager);

    if((MANAGER_MSQID = msgget(IPC_PRIVATE, IPC_CREAT | IPC_EXCL | 0640)) == -1) {
        fprintf(stderr, "Error while creating the message queue.\n");
        fprintf(stderr, "\tError %d: %s\n", errno, strerror(errno));
        return -1;
    }

    /* Initialize the attributes of the threads */
    if(pthread_attr_init(&thr_attr) != 0) {
        fprintf(stderr, "Error while setting the attributes of the threads.\n");
        return -1;
    }

    /* Create all 4 manager threads */

    if((error = pthread_create(&P0, &thr_attr, threadP0, NULL)) != 0) {
        fprintf(stderr, "Error while creating P0.\n");
        fprintf(stderr, "\tError %d: %s\n", error, strerror(error));
        return -1;
    }

    if((error = pthread_create(&P1, &thr_attr, threadP1, NULL)) != 0) {
        fprintf(stderr, "Error while creating P1.\n");
        fprintf(stderr, "\tError %d: %s\n", error, strerror(error));
        return -1;
    }

    if((error = pthread_create(&P2, &thr_attr, threadP2, NULL)) != 0) {
        fprintf(stderr, "Error while creating P2.\n");
        fprintf(stderr, "\tError %d: %s\n", error, strerror(error));
        return -1;
    }

    if((error = pthread_create(&P3, &thr_attr, threadP3, NULL)) != 0) {
        fprintf(stderr, "Error while creating P3.\n");
        fprintf(stderr, "\tError %d: %s\n", error, strerror(error));
        return -1;
    }

    return 0;
}

void processManager(int global_msqid) {

	MANAGER_GLOBAL_MSQID = global_msqid;

	if(initManager() == -1)
        fprintf(stderr, "Error while the initialization of the process manager.\n");

	exitManager(0);
}