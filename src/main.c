#include "main.h"

/* Called when an error occurred */
void exitProgram() {
	/* wait for trains */
	waitpid(TRAIN, NULL, 0);
	/* signal to manager that every train events have been played and terminated */
	kill(MANAGER, SIGINT);
	waitpid(MANAGER, NULL, 0);

	if(msgctl(MSQID, IPC_RMID, NULL) == -1) {
        fprintf(stderr, "Error while deleting the global message queue.\n");
        fprintf(stderr, "\tError %d: %s\n", errno, strerror(errno));
        exit(-1);
    }

	exit(0);
}

/* Handler for the SIGINT signal */
void handlerSIGINT(int num) {
	fprintf(stderr, "\nSIGINT detected: killing all train threads!\n\n");
	fflush(stderr);

	kill(MANAGER, SIGINT);
	kill(TRAIN, SIGINT);
    exitProgram();
}

/* Main function for the program */
int main(const int argc, const char *argv[]) {
	if (argc != 2) {
		printf("Usage :\n");
        printf("\t%s [PATH TO TRAIN FILE]\n", argv[0]);
	} else {
		/* handles interruptions from SIGINT */
		signal(SIGINT, handlerSIGINT);

		MANAGER = -1;
		TRAIN = -1;

		if((MSQID = msgget(IPC_PRIVATE, IPC_CREAT | IPC_EXCL | 0600)) == -1) {
	        fprintf(stderr, "Error while creating the global message queue.\n");
	        fprintf(stderr, "\tError %d: %s\n", errno, strerror(errno));
	        return -1;
    	}

		switch (MANAGER = fork()) {
            case 0 :
				processManager();
				break;
			case -1 :
				exitProgram();
				break;
			default :
				switch (TRAIN = fork()) {
					case 0 :
						processTrain(argv[1]);
						break;
					case -1 :
						exitProgram();
						break;
					default :
						exitProgram();
						break;
				}
				break;
		}

		/* signal handler back to normal */
		signal(SIGINT, SIG_DFL);
	}

	return 0;
}