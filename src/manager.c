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

bool canPassAiguillage(Train t) {
    Direction dir = t.direction;
    TrainType type = t.type;

    if(dir == DIR_WE) { /* west to east */
        switch(type) {
            case TYPE_TGV:
                if(CMP_AIGUILLAGE_2 >= 0 && CMP_LIGNE_TGV >= 0) {
                    CMP_AIGUILLAGE_2++;
                    CMP_LIGNE_TGV++;
                    return true;
                }
                break;
            case TYPE_GL:
                if(CMP_AIGUILLAGE_2 >= 0 && CMP_LIGNE_GL >= 0) {
                    CMP_AIGUILLAGE_2++;
                    CMP_LIGNE_GL++;
                    return true;
                }
                break;
            case TYPE_M:
                if(CMP_AIGUILLAGE_1 >= 0) {
                    CMP_AIGUILLAGE_1++;
                    return true;
                }
                break;
        }
    } else {
        switch(type) {
            case TYPE_TGV:
                if(CMP_AIGUILLAGE_2 <= 0) {
                    CMP_AIGUILLAGE_2--;
                    CMP_LIGNE_TGV++;
                    return true;
                }
                break;
            case TYPE_GL:
                if(CMP_AIGUILLAGE_2 <= 0) {
                    CMP_AIGUILLAGE_2--;
                    CMP_LIGNE_GL++;
                    return true;
                }
                break;
            case TYPE_M:
                if(CMP_AIGUILLAGE_1 <= 0) {
                    CMP_AIGUILLAGE_1--;
                    return true;
                }
                break;
        }
    }

    return false;
}

bool canPassTunnel(Train t) {

    Direction dir = t.direction;
    TrainType type = t.type;

    if(dir == DIR_WE) { /* west to east */
        if(CMP_TUNNEL >= 0) {
            CMP_TUNNEL++;
            switch(type) {
                case TYPE_TGV:
                    CMP_LIGNE_TGV--;
                    break;
                case TYPE_GL:
                    CMP_LIGNE_GL--;
                    break;
                default:
                    break;
            }
            return true;
        }
    } else {
        if(CMP_TUNNEL <= 0) {
            CMP_TUNNEL--;
            switch(type) {
                case TYPE_TGV:
                    if(CMP_LIGNE_TGV <= 0)
                        CMP_LIGNE_TGV--;
                    break;
                case TYPE_GL:
                    if(CMP_LIGNE_GL <= 0)
                        CMP_LIGNE_GL--;
                    break;
                default:
                    break;
            }
            return true;
        }
    }

    return false;
}

void sendPermission(MessageType type, Message* msg) {
    int dst = msg->dst;
    msg->dst = msg->src;
    msg->type = type;
    msg->src = dst;
    msgsnd(MSQID, &msg, sizeof(Message) - sizeof(long), 0);
}

void resolveRequest(char* name, MessageList* ml) {
    int i;
    bool solved;
    Message msg;

    for (i = 0; i < ml->size; i++) {
        solved = false;
        msg = poll(ml);
        if(msg.train.type == TYPE_TGV) {
            if(msg.dst == ID_TUNNEL) {
                if(canPassTunnel(msg.train)) {
                    printf("%s : Sending permission to TGV %d.\n", name, msg.dst);
                    sendPermission(MSG_PERMISSION, &msg);
                    solved = true;
                }
            } else {
                if(canPassAiguillage(msg.train)) {
                    printf("%s : Sending permission to TGV %d.\n", name, msg.dst);
                    sendPermission(MSG_PERMISSION, &msg);
                    solved = true;
                }
            }
        }
        if(solved == false)
            offer(ml, msg);
    }

    for (i = 0; i < ml->size; i++) {
        solved = false;
        msg = poll(ml);
        if(msg.train.type == TYPE_GL) {
            if(msg.dst == ID_TUNNEL) {
                if(canPassTunnel(msg.train)) {
                    printf("%s : Sending permission to GL %d.\n", name, msg.dst);
                    sendPermission(MSG_PERMISSION, &msg);
                    solved = true;
                }
            } else {
                if(canPassAiguillage(msg.train)) {
                    printf("%s : Sending permission to GL %d.\n", name, msg.dst);
                    sendPermission(MSG_PERMISSION, &msg);
                    solved = true;
                }
            }
        }
        if(solved == false)
            offer(ml, msg);
    }

    for (i = 0; i < ml->size; i++) {
        solved = false;
        msg = poll(ml);
        if(msg.train.type == TYPE_M) {
            if(msg.dst == ID_TUNNEL) {
                if(canPassTunnel(msg.train)) {
                    printf("%s : Sending permission to M %d.\n", name, msg.dst);
                    sendPermission(MSG_PERMISSION, &msg);
                    solved = true;
                }
            } else {
                if(canPassAiguillage(msg.train)) {
                    printf("%s : Sending permission to M %d.\n", name, msg.dst);
                    sendPermission(MSG_PERMISSION, &msg);
                    solved = true;
                }
            }
        }
        if(solved == false)
            offer(ml, msg);
    }
}

void resolveNotification(Message msg) {
    pthread_mutex_lock(&MUTEX_COUNT);
    int modif;
    if(msg.train.direction == DIR_WE)
        modif = 1;
    else
        modif = -1;

    pthread_mutex_lock(&MUTEX_COUNT);
    switch(msg.dst) {
        case ID_AIGUILLAGE_1:
            CMP_AIGUILLAGE_1 -= modif;
            break;
        case ID_AIGUILLAGE_2:
            CMP_AIGUILLAGE_2 -= modif;
            break;
        case ID_TUNNEL:
            CMP_TUNNEL -= modif;
            break;
        default:
            break;
    }
    pthread_mutex_lock(&MUTEX_COUNT);
}

void* managerThread(void* arg) {
	char name[30]="";
    long permissionFreq = ((ManagerThreadArg*)arg)->permissionFreq;
    int id = ((ManagerThreadArg*)arg)->id;
    int i;
    Message msg;
    MessageList* ml = ((ManagerThreadArg*)arg)->messageList;

	strcpy(name, ((ManagerThreadArg*)arg)->name);

	printf("%s : My id is %d.\n", name, id);

    while(1) {
    	usleep(permissionFreq);
    	while (msgrcv(MSQID, &msg, sizeof(Message) - sizeof(long), id, IPC_NOWAIT) != -1) {
	        switch (msg.type) {
	            case MSG_REQUEST :
	                printf("%s : Request received from %d.\n", name, msg.src);
	                offer(ml, msg); /* store request in message queue */
	                break;
	            case MSG_NOTIFICATION :
	            	printf("%s : Notification received from %d.\n", name, msg.src);
                    resolveNotification(msg);
	                break;
                default :
                    fprintf(stderr, "Error: you're not supposed to go there");
	        }
    	}

    	if(ml->size != 0) /* if there are messages to read */
    		resolveRequest(name, ml);
    }
}

void exitManager(int num) {
    int error;

    /* free memory allocated by message lists*/
    removeList(&A1MSG);
    removeList(&A2MSG);
    removeList(&TMSG);

    /* join threads */

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

    /* init argument passed to manager threads */
    ManagerThreadArg a1, a2, t;

    /* initialize mutex for counters */
    pthread_mutex_init(&MUTEX_COUNT, NULL);

    /* initialize mutex for all five counters */
    pthread_mutex_lock(&MUTEX_COUNT);
    CMP_AIGUILLAGE_1 = 0;
    CMP_AIGUILLAGE_2 = 0;
    CMP_TUNNEL = 0;
    CMP_LIGNE_GL = 0;
    CMP_LIGNE_TGV = 0;
    pthread_mutex_unlock(&MUTEX_COUNT);

    /* handles interruption from SIGINT */
	signal(SIGINT, managerHandlerSIGINT);

    /* creation of AIGUILLAGE 2 thread */

    strcpy(a1.name, "AIGUILLAGE 1");
    a1.id = ID_AIGUILLAGE_1;
    a1.permissionFreq = 500000;
    A1MSG = initList();
    a1.messageList = &A1MSG;

    if((error = pthread_create(&AIGUILLAGE1, NULL, managerThread, (void*)&a1)) != 0) {
        fprintf(stderr, "Error while creating AIGUILLAGE 1.\n");
        fprintf(stderr, "\tError %d: %s\n", error, strerror(error));
        return -1;
    }

    /* creation of AIGUILLAGE 2 thread */

    strcpy(a2.name, "AIGUILLAGE 2");
    a2.id = ID_AIGUILLAGE_2;
    a2.permissionFreq = 500000;
    A1MSG = initList();
    a2.messageList = &A2MSG;

    if((error = pthread_create(&AIGUILLAGE2, NULL, managerThread, (void*)&a2)) != 0) {
        fprintf(stderr, "Error while creating AIGUILLAGE 2.\n");
        fprintf(stderr, "\tError %d: %s\n", error, strerror(error));
        return -1;
    }

    /* creation of TUNNEL thread */

    strcpy(t.name, "TUNNEL");
    t.id = ID_TUNNEL;
    t.permissionFreq = 500000;
    TMSG = initList();
    t.messageList = &TMSG;

    if((error = pthread_create(&TUNNEL, NULL, managerThread, (void*)&t)) != 0) {
        fprintf(stderr, "Error while creating tunnel.\n");
        fprintf(stderr, "\tError %d: %s\n", error, strerror(error));
        return -1;
	}

    /* wait 1s to be sure that threads had time to make a copy of their thread argument*/
	usleep(1000000);

    return 0;
}

void processManager() {
	if(initManager() == -1)
        fprintf(stderr, "Error during the initialization of the process manager.\n");

	exitManager(0);
}