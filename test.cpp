#include "essentials.h"

int main(int argc, char* argv[]) {
	int p[2];
	pipe(p);

	if (fork() == 0) {
		dup2(p[1],1);
		dup2(p[1],2);
		close(p[1]);
		close(p[0]);


		if ((execl("/bin/ls","ls")) == 1 ) {
			perror(SHELLNAME);
		}
		exit(EXIT_SUCCESS);
	}
	else {
		close(p[1]);

		sleep(100);
		char buffer[1024];
		while (read(p[0], buffer, sizeof(buffer)) != 0) {
			cout<<buffer<<endl;
		}
		close(p[0]);
	}
}