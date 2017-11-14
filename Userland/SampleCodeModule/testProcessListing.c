#include "tests.h"

void processDead() {
	printf("Dead.\n");
	LoopNop();
}

void processListed() {
	printf("Hello!, I am process: %d\n", getPid());
	int pid = createProcess(&processDead);
	LoopNop();
}
int lastPID = 0;
void processListing() {

	int pid;

	for (int i = 0; i < 4; i++) {
		printf("I am process %d\n", getPid());
		pid = createProcess(&processListed);
	}
	lastPID = pid + 1;
}

void killListing() {
	for (int i = 0; i < 8; i++){
		killProcess(lastPID - i);
	}
}