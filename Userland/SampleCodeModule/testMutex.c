#include "libc.h"
#include "commands.h"
#include "tests.h"

int x;

void processThatAcquires() {

	int pid = getPid();

	printf("Hi! I am process %d acquiring bolt...\n", pid);

	down();
	printf("Process %d acquired bolt\n", pid);

	x++;
	printf("Process %d does: x = %d.\n", pid, x);

	LoopNop();

}

void processMutualExclusionSetup() {

	int pid2 = createProcess(&processThatAcquires);

	int pid3 = createProcess(&processThatAcquires);

	int pid4 = createProcess(&processThatAcquires);

	for (int i = 0; i < 400000000; i++);

	up();

	for (int i = 0; i < 400000000; i++);

	up();

	killProcess(pid2);
	killProcess(pid3);
	killProcess(pid4);

}