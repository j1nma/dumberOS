#include "libc.h"
#include "commands.h"
#include "tests.h"

int x;

void processMutualExclusion() {

	if (!(getPid() % 2)) {
		printf("Hi! I am process %d acquiring bolt.\n", getPid());

		down();

		x++;
		printf("x = %d.\n", x);

	} else {
		printf("Hi! I am process %d acquiring bolt.\n", getPid());

		down();

		x++;
		printf("x = %d.\n", x);
	}

	LoopNop();

}

void processMutualExclusionSetup() {

	for (int i = 0; i < 2; i++) {
		printf("I am process %d\n", getPid());
		int pid = createProcess(&processMutualExclusion);
		printf("Process %d was created.\n", pid);
	}

	LoopNop();

}