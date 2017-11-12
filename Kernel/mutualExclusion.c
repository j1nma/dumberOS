#include "mutualExclusion.h"
#include "drivers.h"
#include "scheduler.h"
#include "memory.h"

int * newBolt() {
	int * ptr = (int*)(memAlloc(sizeof(int)));
	*ptr = 0;
	return ptr;
}

int acquireBolt(int * bolt) {

	if (__sync_val_compare_and_swap (bolt, 0, 1) == 1) {

		write("Blocked by bolt...\n", 20);

		blockCurrent(BOLT_BLOCK);

		unblock(BOLT_BLOCK);
	}

	return 0;
}

int releaseBolt(int * bolt) {
	*bolt = 0;
	return 0;
}

int deleteBolt(int * bolt) {
	memFree(bolt);
	return 0;
}