#include "manager.h"

void managerHandlerSIGINT(int num) {
    /* inform child process that they must finish their execution */
    
    /* leave time to manager threads to finish reading their message queue */
    usleep(2000000);

    pthread_kill(AIGUILLAGE1, SIGTERM);
    pthread_kill(AIGUILLAGE2, SIGTERM);
    pthread_kill(TUNNEL, SIGTERM);
    exitProgram();
}

bool canPassGarage(Train t) {
    Direction dir = t.direction;
    TrainType type = t.type;
    Position pos = t.position;

    if (dir == DIR_WE) { /* west to east */
        if (pos == POS_VOIEA || pos == POS_VOIEC) { /* located west from garage */
            if (type == TYPE_TGV && LIGNETGV_WE < 0) {
                return false;
            } else if (type == TYPE_GL && LIGNEGL_WE < 0) {
                return false;
            }
        }
    } else {
        if (type == TYPE_TGV && LIGNETGV_WE > 0) {
            return false;
        } else if (type == TYPE_GL && LIGNEGL_WE > 0) {
            return false;
        }
    }

    return true;
}

bool canPassLigne(Train t) {
    Direction dir = t.direction;
    TrainType type = t.type;
    Position pos = t.position;

    if (dir == DIR_WE) { /* west to east */
        if (LIGNE_WE < 0) {
            return false;
        }
    } else {
        if (pos == POS_LIGNE && LIGNE_WE > 0) {
            return false;
        }
    }

    return true;
}

void setDirectionCounters(Train t) {
    Direction dir = t.direction;
    TrainType type = t.type;
    Position pos = t.position;

    if (dir == DIR_WE) { /* west to east */
        if (pos == POS_VOIEA || pos == POS_VOIEC) { /* located west from garage */
            if (type == TYPE_TGV) {
                LIGNETGV_WE++;
            } else if (type == TYPE_GL) {
                LIGNEGL_WE++;
            }
            LIGNE_WE++;
        }
    } else {
        if (pos == POS_LIGNE) {
            LIGNE_WE--;
        }
        if (type == TYPE_TGV) {
            LIGNETGV_WE--;
        } else if (type == TYPE_GL) {
            LIGNEGL_WE--;
        }
    }
}

void unsetDirectionCounters(Train t) {
    Direction dir = t.direction;
    TrainType type = t.type;
    Position pos = t.position;

    if (dir == DIR_WE) { /* west to east */
        if (pos == POS_LIGNE) {
            LIGNE_WE--;
            if (type == TYPE_TGV) {
                LIGNETGV_WE--;
            } else if (type == TYPE_GL) {
                LIGNEGL_WE--;
            }
        }
    } else {
        if (pos == POS_VOIEB || pos == POS_VOIED) {
            if (type == TYPE_TGV) {
                LIGNETGV_WE++;
            } else if (type == TYPE_GL) {
                LIGNEGL_WE++;
            }
        } else if (pos == POS_LIGNEM_EW || pos == POS_LIGNETGV || pos == POS_LIGNEGL) {
            LIGNE_WE++;
        }
    }
}

void* managerThread(void* arg) {
	char name[30]="";
    long permissionFreq = ((ManagerThreadArg*)arg)->permissionFreq;
    int id = ((ManagerThreadArg*)arg)->id;
    int i;
    Message msg, tmpMsg;
    MessageList* ml = ((ManagerThreadArg*)arg)->messageList;
    int passingTrain = 0;

	strcpy(name, ((ManagerThreadArg*)arg)->name);

	printf("%s : My id is %d.\n", name, id);

    if (id == 2) {
        printf("set train WE on ligne\n");
        LIGNE_WE --;
        usleep(4000000);
        printf("unset train WE on ligne\n");
        LIGNE_WE ++;
    }

    while(1) {
    	usleep(permissionFreq);
    	while (msgrcv(MSQID, &msg, sizeof(Message) - sizeof(long), id, IPC_NOWAIT) != -1) {
	        switch (msg.type) {
	            case MSG_REQUEST_FORCE :
	                printf("%s : Receive force request from train %li.\n", name, msg.src);
	                passingTrain++;
	                break;
	            case MSG_REQUEST :
	                printf("%s : Receive request from %li.\n", name, msg.src);
	                offer(ml, msg); /* store request in message queue */
	                break;
	            case MSG_NOTIFICATION :
	            	printf("%s : Receive notification from %li.\n", name, msg.src);
                    pthread_mutex_lock(&DIR_MUTEX);
                    unsetDirectionCounters(msg.train);
                    pthread_mutex_unlock(&DIR_MUTEX);
	            	passingTrain--;
	                break;
                default :
                    fprintf(stderr, "Error: you're not supposed to go there");
	        }
    	}
    	if (passingTrain == 0 && ml->size != 0) { /* if there is no train on the critical railway */
            /* priority management : first lowest priority that can pass */

            pthread_mutex_lock(&DIR_MUTEX);
            msg.type = TYPE_M;
    		for (i = 0; i < ml->size; i++) {
                tmpMsg = poll(ml);
                if (tmpMsg.type < msg.type && canPassGarage(tmpMsg.train) && canPassLigne(tmpMsg.train)) {
                    msg = tmpMsg;
                }
                offer(ml, tmpMsg);
            }

            if (canPassGarage(msg.train) && canPassLigne(msg.train)) {
                setDirectionCounters(msg.train);
            }

            pthread_mutex_unlock(&DIR_MUTEX);


            /* put the list in its original chronologic order */

            for (i = 0; i < ml->size; i++) {
                tmpMsg = poll(ml);
                if (msg.src != tmpMsg.src) {
                    offer(ml, tmpMsg);
                }
            }

            /* sending permission to selected train */
        if (canPassGarage(msg.train) && canPassLigne(msg.train)) {
            passingTrain++;
            msg.dst = msg.src;
            msg.src = id;
            msg.type = MSG_PERMISSION;
            printf("%s : Send permission to %li.\n", name, msg.dst);
            msgsnd(MSQID, &msg, sizeof(Message) - sizeof(long), 0);
            }
    	}
    }

	pthread_exit(NULL);
}

void exitManager(int num) {

    /* Join all 4 manager threads */
    int error;

    /* free memory allocated by message lists*/
    removeList(&A1MSG);
    removeList(&A2MSG);
    removeList(&TMSG);

    /*join threads*/

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

    /* init counters */
    LIGNETGV_WE = 0;
    LIGNEGL_WE = 0;
    LIGNE_WE = 0;

    /* handles interruption from SIGINT */
	signal(SIGINT, managerHandlerSIGINT);

    /* Create all 4 manager threads */

    /* init argument passed to manager threads*/
    ManagerThreadArg a1, a2, t;
    strcpy(a1.name, "AIGUILLAGE 1");
    a1.id = 1;
    a1.permissionFreq = 500000;
    A1MSG = initList();
    a1.messageList = &A1MSG;

    if((error = pthread_create(&AIGUILLAGE1, NULL, managerThread, (void*)&a1)) != 0) {
        fprintf(stderr, "Error while creating aiguillage1.\n");
        fprintf(stderr, "\tError %d: %s\n", error, strerror(error));
        return -1;
    }

    strcpy(a2.name, "AIGUILLAGE 2");
    a2.id = 2;
    a2.permissionFreq = 500000;
    A1MSG = initList();
    a2.messageList = &A2MSG;

    if((error = pthread_create(&AIGUILLAGE2, NULL, managerThread, (void*)&a2)) != 0) {
        fprintf(stderr, "Error while creating aiguillage2.\n");
        fprintf(stderr, "\tError %d: %s\n", error, strerror(error));
        return -1;
    }

    strcpy(t.name, "TUNNEL");
    t.id = 3;
    t.permissionFreq = 500000;
    TMSG = initList();
    t.messageList = &TMSG;

    if((error = pthread_create(&TUNNEL, NULL, managerThread, (void*)&t)) != 0) {
        fprintf(stderr, "Error while creating tunnel.\n");
        fprintf(stderr, "\tError %d: %s\n", error, strerror(error));
        return -1;
	}

    /* wait 2 s to be sure that threads had time to make a copy of their thread argument*/
	usleep(2000000);

    return 0;
}

void processManager(int global_msqid) {
	if(initManager() == -1)
        fprintf(stderr, "Error during the initialization of the process manager.\n");

	exitManager(0);
}