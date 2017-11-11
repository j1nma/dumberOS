#include "tests.h"

void processListed() {
	printf("Hello!, I am process: %d\n", getPid());
	LoopNop();
}

void processListing() {

	int pid;

	for (int i = 0; i < 4; i++) {
		printf("I am process %d\n", getPid());
		pid = createProcess(&processListed);
	}

}