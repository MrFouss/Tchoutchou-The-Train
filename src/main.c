#include "main.h"

void handlerSIGINT(int num) {
	if (getpid() == program) {
		exitProgram();
	}
}

//called when an error occured
void exitProgram() {
	//ignore interruptions while end the program
	signal(SIGINT, SIG_IGN);

	//TODO inform every child process of the termination
	//TODO remove message queue
	// /!\ check the existance of process before sending them signals in case of an error
	
	exit(0);
}

int main(int argc, char const *argv[]) {
	if (argc != 2) {
		printf("Expected command line :\n ./exec <train file>\n");
	} else {
		//handle interruption
		signal(SIGINT, handlerSIGINT);

        program = getpid();

		//TODO init queue

		switch (manager = fork()) {
			case 0 :
				processManager(msqid);
				break;
			case -1 :
				exitProgram();
				break;
			default :
				switch (train = fork()) {
					case 0 :
						processTrain(msqid, argv[1]);
						break;
					case -1 :
						exitProgram();
						break;
					default :
						//wait for manager and train
						waitpid(train, NULL, 0);
						waitpid(manager, NULL, 0);
				}
				break;
		}

		//signal handler back to normal
		signal(SIGINT, SIG_DFL);
	}

	return 0;
}