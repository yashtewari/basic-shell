#include "essentials.h"

void *handlerThreadMain(void *thread_argument);

class exitsHandler {
	private:
		pthread_t threadId;
		int temp;
	public:
		exitsHandler() {
			if (pthread_create(&threadId, NULL, handlerThreadMain, &temp)) {
				perror(SHELLNAME);
			}
		}

};

void *handlerThreadMain(void *thread_argument) {
	siginfo_t exitedChildInfo;
	while (true) {
		if (waitid(P_ALL, -1, &exitedChildInfo, WEXITED) != -1) {
			cout<<"Child with PID "<<exitedChildInfo.si_pid<<" terminated."<<endl;
		}
		sleep(1);
	}
}