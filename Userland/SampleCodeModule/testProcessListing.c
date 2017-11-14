#include "tests.h"

void processListed() {
	printf("Hello!, I am process: %d\n", getPid());
	LoopNop();
}
int lastPID = 0;
void processListing() {

	int pid;

	for (int i = 0; i < 4; i++) {
		printf("I am process %d\n", getPid());
		pid = createProcess(&processListed);
	}
	lastPID = pid;
}

void killListing() {
	for (int i = 0; i < 4; i++){
		killProcess(lastPID - i);
	}
}