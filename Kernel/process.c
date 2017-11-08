#include "process.h"
#include <drivers.h>

typedef int (*EntryPoint)();

static void * const sampleCodeModuleAddress = (void*)0x400000;

void callProcess(struct process * process) {
	endInter();
	switchStackAndJump((process->userStack), (process->entryPoint));
}

int createNewProcess(void * statringPoint) {

	struct process * processN;
	processN = malloc(sizeof(struct process));
	processN->entryPoint = statringPoint;
	processN->userStack = toStackAddress(malloc(0x1000));
	processN->kernelStack = toStackAddress(malloc(0x1000));
	processN->flippedStack = toStackAddress(malloc(0x1000));
	processN->flipped = 0;
	processN->pid = -1;

	/* IPC */
	processN->receiver_buffer = (Queue *)malloc(sizeof(Queue));
	queueInit(processN->receiver_buffer, (MESSAGE_SIZE + 1) * sizeof(char));
	/* IPC */

	queueProcess(processN);
	return 2;
}

void * toStackAddress(void * page) {
	return page + 0x1000 - 0x10;
}