#include "libc.h"
#include "commands.h"
#include "tests.h"

void processThatAcquiresMulti3() {
	int pid = getPid();

	printf("Hi! I am process %d.\n", pid);

	int *boltPtr_2 = (void *)receive();

	int * bolt2 = *boltPtr_2;


	int *boltPtr_3 = (void *)receive();

	int * bolt3 = *boltPtr_3;

	printf("Will aquire bolt 2 and 3.\n");
	down(bolt2);
	down(bolt3);

	printf("Aquired bolt 2 and 3. Waiting to die...\n");

	LoopNop();

}

void processThatAcquiresMulti2() {
	int pid = getPid();

	printf("Hi! I am process %d.\n", pid);

	int *boltPtr_1 = (void *)receive();

	int * bolt1 = *boltPtr_1;

	printf("Will aquire bolt 1.\n");
	down(bolt1);

	printf("Aquired bolt 1. Waiting to die...\n");

	LoopNop();

}


void processThatAcquiresMulti1() {
	int pid = getPid();

	printf("Hi! I am process %d.\n", pid);

	int *boltPtr_1 = (void *)receive();

	int * bolt1 = *boltPtr_1;

	int *boltPtr_2 = (void *)receive();

	int * bolt2 = *boltPtr_2;

	int *boltPtr_3 = (void *)receive();

	int * bolt3 = *boltPtr_3;

	down(bolt1);
	down(bolt2);
	down(bolt3);

	printf("Aquired all 3 bolts.\n");

	for (int i = 0; i < 20000000; ++i);

	printf("Releasing bolt 1.\n");
	up(bolt1);

	for (int i = 0; i < 20000000; ++i);

	printf("Releasing bolt 2.\n");
	up(bolt2);

	for (int i = 0; i < 20000000; ++i);

	printf("Releasing bolt 3. Waiting to die...\n");
	up(bolt3);

	LoopNop();

}

void processMultiMutualExclusionSetup() {

	int * bolt1 = (int *)createBolt();
	int * bolt2 = (int *)createBolt();
	int * bolt3 = (int *)createBolt();

	int pid1 = createProcess(&processThatAcquiresMulti1);

	send(&bolt1, pid1);
	send(&bolt2, pid1);
	send(&bolt3, pid1);

	for (int i = 0; i < 2000000; i++);

	int pid2 = createProcess(&processThatAcquiresMulti2);

	int pid3 = createProcess(&processThatAcquiresMulti3);

	

	send(&bolt1, pid2);

	send(&bolt2, pid3);
	send(&bolt3, pid3);


	for (int i = 0; i < 500000000; i++);

	killProcess(pid1);
	killProcess(pid2);
	killProcess(pid3);

	freeBolt(bolt1);
	freeBolt(bolt2);
	freeBolt(bolt3);

}