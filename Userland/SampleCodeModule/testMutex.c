#include "libc.h"
#include "commands.h"
#include "tests.h"

int x;

void processThatAcquires() {

	int pid = getPid();

	printf("Hi! I am process %d acquiring bolt...\n", pid);


	int *boltPtr = (void *)receive();

	int * bolt = *boltPtr;


	down(bolt);
	printf("Process %d acquired bolt\n", pid);

	x++;
	printf("Process %d does: x = %d.\n", pid, x);

	LoopNop();

}

void processMutualExclusionSetup() {

	int * bolt = (int *)createBolt();

	int pid2 = createProcess(&processThatAcquires);

	int pid3 = createProcess(&processThatAcquires);

	int pid4 = createProcess(&processThatAcquires);

	send(&bolt, pid2);
	send(&bolt, pid3);
	send(&bolt, pid4);

	for (int i = 0; i < 400000000; i++);

	up(bolt);

	for (int i = 0; i < 400000000; i++);

	up(bolt);



	for (int i = 0; i < 20000000; i++);

	killProcess(pid2);
	killProcess(pid3);
	killProcess(pid4);

	freeBolt(bolt);

}