#include "process.h"
#include "scheduler.h"
#include "interruptions.h"
#include <drivers.h>

typedef int (*EntryPoint)();


void callProcess(struct process * process) {
	endInter();

	enableTickInter();

	switchStackAndJump((process->userStack), (process->entryPoint));
}

struct process * createNewProcess(void * statringPoint, int parent) {

	struct process * processN = malloc(sizeof(struct process));
	processN->entryPoint = statringPoint;
	processN->userStack = toStackAddress(malloc(0x1000));
	processN->kernelStack = toStackAddress(malloc(0x1000));
	processN->flippedStack = toStackAddress(malloc(0x1000));
	processN->flipped = 0;
	processN->pid = createPid();
	processN->parent = parent;

	/* IPC */
	processN->receiver_buffer = (Queue *)malloc(sizeof(Queue));
	queueInit(processN->receiver_buffer, (MESSAGE_SIZE + 1) * sizeof(char));
	/* IPC */

	return processN;
}

void * toStackAddress(void * page) {
	return page + 0x1000 - 0x10;
}