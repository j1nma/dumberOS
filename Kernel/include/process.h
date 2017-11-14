#ifndef _PROCESS_H
#define _PROCESS_H

#include "queue.h"
#include "scheduler.h"
#include "shared_memory.h"
#define MESSAGE_SIZE 20

// https://unix.stackexchange.com/questions/80038/what-is-the-structure-of-a-linux-process

struct process {

	int pid;

	int state;

	void * userStackStart;

	void * kernelStackStart;

	void * flippedStackStart;

	void * userStack;

	void * kernelStack;

	void * flippedStack;

	void * entryPoint;

	int flipped;

	int parent;

	Queue * receiver_buffer;

	struct shared_mem * sm;
};

void callProcess(struct process * process);

int isBlocked(struct process * process);

struct process * createNewProcess(void * statringPoint, int parent);

void freeProcess(struct process * process);

void * toStackAddress(void * page, int pagesPerStack);

#endif