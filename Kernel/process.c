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

	int pagesPerStack = 4;

	struct process * processN = malloc(sizeof(struct process));
	processN->entryPoint = statringPoint;


	processN->userStackStart = allocNPages(pagesPerStack);
	processN->kernelStackStart = allocNPages(pagesPerStack);
	processN->flippedStackStart = allocNPages(pagesPerStack);


	processN->userStack = toStackAddress(processN->userStackStart, pagesPerStack);
	processN->kernelStack = toStackAddress(processN->kernelStackStart, pagesPerStack);
	processN->flippedStack = toStackAddress(processN->flippedStackStart, pagesPerStack);
	processN->flipped = 0;
	processN->pid = createPid();
	processN->parent = parent;

	/* IPC */
	processN->receiver_buffer = (Queue *)malloc(sizeof(Queue));
	queueInit(processN->receiver_buffer, (MESSAGE_SIZE + 1) * sizeof(char));
	/* IPC */

	return processN;
}

void freeProcess(struct process * process) {

	freeSpace(process->userStackStart);
	freeSpace(process->kernelStackStart);
	freeSpace(process->flippedStackStart);

	free(process->receiver_buffer);
	free(process);

}

void * toStackAddress(void * page, int pagesPerStack) {
	return page + pagesPerStack * 0x1000 - 0x10;
}