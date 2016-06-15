#include <signal.h>
#include "main.h"

/**
 * Called when an error occurred
 */
void exitProgram() {
	/* ignore interruptions while the program is ending */
	signal(SIGINT, SIG_IGN);

	/* wait for manager and train */
	waitpid(TRAIN, NULL, 0);
	/* signal to manager that every train events ave been played and terminated*/
	kill(MANAGER, SIGINT);
	waitpid(MANAGER, NULL, 0);

	if(msgctl(MSQID, IPC_RMID, NULL) == -1) {
        fprintf(stderr, "Error while deleting the global message queue.\n");
        fprintf(stderr, "\tError %d: %s\n", errno, strerror(errno));
        exit(-1);
    }

	exit(0);
}

/**
 * Handler for the SIGINT signal
 */
void handlerSIGINT(int num) {
	/* inform child process that they must finish their execution */
	if (MANAGER != -1) {
		kill(MANAGER, SIGINT);
	}
	if (TRAIN != -1) {
		kill(TRAIN, SIGINT);
	}
    exitProgram();
}

/**
 * Main function for the program
 */
int main(const int argc, const char *argv[]) {
	if (argc != 2) {
		printf("Usage :\n");
        printf("\t%s [PATH TO TRAIN FILE]\n", argv[0]);
	} else {
		/* handles interruptions from SIGINT */
		signal(SIGINT, handlerSIGINT);

        PROGRAM = getpid();

		if((MSQID = msgget(IPC_PRIVATE, IPC_CREAT | IPC_EXCL | 0600)) == -1) {
	        fprintf(stderr, "Error while creating the global message queue.\n");
	        fprintf(stderr, "\tError %d: %s\n", errno, strerror(errno));
	        return -1;
    	}

		switch (MANAGER = fork()) {
            case 0 :
				processManager(MSQID);
				break;
			case -1 :
				exitProgram();
				break;
			default :
				switch (TRAIN = fork()) {
					case 0 :
						processTrain(MSQID, argv[1]);
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

		/* Signal handler back to normal */
		signal(SIGINT, SIG_DFL);
	}

	return 0;
}