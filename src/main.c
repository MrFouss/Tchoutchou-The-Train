#include "main.h"

/**
 * Called when an error occurred
 */
void exitProgram() {
	// ignore interruptions while the programis ending */
	signal(SIGINT, SIG_IGN);

	// TODO inform every child process of the termination
	// TODO remove message queue
	// TODO check the existence of process before sending them signals in case of an error
	
	exit(0);
}

/**
 * Handler for the SIGINT signal
 */
void handlerSIGINT(int num) {
	if (getpid() == PROGRAM)
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
		// handles interruptions from SIGINT
		signal(SIGINT, handlerSIGINT);

        PROGRAM = getpid();

		// TODO init queue

		switch (MANAGER = fork()) {
            case 0 :
				processManager(MSQID, argv[0]);
				break;
			case -1 :
				exitProgram();
				break;
			default :
				switch (TRAIN = fork()) {
					case 0 :
						// processTrain(MSQID, argv[1]);
						break;
					case -1 :
						exitProgram();
						break;
					default :
						//wait for manager and train
						waitpid(TRAIN, NULL, 0);
						waitpid(MANAGER, NULL, 0);
				}
				break;
		}

		// signal handler back to normal
		signal(SIGINT, SIG_DFL);
	}

	return 0;
}