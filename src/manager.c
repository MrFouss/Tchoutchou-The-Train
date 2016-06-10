#include "manager.h"

void* threadP0(void* arg) {
	// TODO
	pthread_exit(NULL);
}

void* threadP1(void* arg) {
	// TODO
	pthread_exit(NULL);
}

void* threadP2(void* arg) {
	// TODO
	pthread_exit(NULL);
}

void* threadP3(void* arg) {
	// TODO
	pthread_exit(NULL);
}

void exitManager(int num) {

    // Join all 4 manager threads

    if(pthread_join(P0, NULL) != 0) {
        fprintf(stderr, "Error while joining P0.\n");
        exit(-1);
    }

    if(pthread_join(P1, NULL) != 0) {
        fprintf(stderr, "Error while joining P1.\n");
        exit(-1);
    }

    if(pthread_join(P2, NULL) != 0) {
        fprintf(stderr, "Error while joining P2.\n");
        exit(-1);
    }

    if(pthread_join(P3, NULL) != 0) {
        fprintf(stderr, "Error while joining P3.\n");
        exit(-1);
    }

    // Close the descriptor
    if(mq_close(MANAGER_MSQID) == -1) {
        fprintf(stderr, "Error while closing the descriptor.\n");
        exit(-1);
    }

    // Delete the message queue
    if(mq_unlink(QNAME) == -1) {
        fprintf(stderr, "Error while deleting the message queue.\n");
        exit(-1);
    }

    exit(0);
}

int initManager() {
	struct mq_attr msq_attr;
    pthread_attr_t thr_attr;

    // handles interruption from SIGINT
	signal(SIGINT, exitManager);

    msq_attr.mq_flags = 0;
    msq_attr.mq_msgsize = MSGSIZE;
    msq_attr.mq_maxmsg = MAXMSG;
    msq_attr.mq_curmsgs = 0;

    // Delete the message queue if it exists
    if(mq_unlink(QNAME) == 0)
        printf("Warning: message queue %s deleted.\n", QNAME);

    // Create the message queue
    if((MANAGER_MSQID = mq_open(QNAME, O_RDWR | O_CREAT, S_IRWXU, &msq_attr)) == (mqd_t) -1) {
        fprintf(stderr, "Error during the creation of the message queue.\n");
        return -1;
    }

    // Initialize the attributes of the threads
    if(pthread_attr_init(&thr_attr) != 0) {
        fprintf(stderr, "Error while setting the attributes of the threads.\n");
        return -1;
    }

    // Create all 4 manager threads

    if(pthread_create(&P0, &thr_attr, threadP0, NULL) == 0) {
        fprintf(stderr, "Error while creating P0.\n");
        return -1;
    }

    if(pthread_create(&P1, &thr_attr, threadP1, NULL) == 0) {
        fprintf(stderr, "Error while creating P1.\n");
        return -1;
    }

    if(pthread_create(&P2, &thr_attr, threadP2, NULL) == 0) {
        fprintf(stderr, "Error while creating P2.\n");
        return -1;
    }

    if(pthread_create(&P3, &thr_attr, threadP3, NULL) == 0) {
        fprintf(stderr, "Error while creating P3.\n");
        return -1;
    }

    return 0;
}

void processManager(int global_msqid) {

	MANAGER_GLOBAL_MSQID = global_msqid;

	if(initManager() == -1)
        fprintf(stderr, "Error while the initialization of the process manager.\n");

	exitManager(NULL);
}