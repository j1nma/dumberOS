#include "tests.h"

void processKilled() {
	printf("Hello!, I am process: %d\n", getPid());
	iam();
}

void processKiller() {
	printf("Hello!, I am process: %d\n", getPid());
	int pid = createProcess(&processKilled);
	printf("Created process: %d\n", pid);
	for (int i = 0; i < 1000000000; ++i);

	killProcess(pid);
	printf("Killed process: %d\n", pid);

	iam();
}