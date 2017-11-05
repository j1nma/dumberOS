#ifndef _PROCESS_H
#define _PROCESS_H

#include "circular_buffer.h"
#include "queue.h"

// https://unix.stackexchange.com/questions/80038/what-is-the-structure-of-a-linux-process

struct process {

	int pid;

	int state;

	int priority;

	void * userStack;

	void * kernelStack;

	void * entryPoint;

	int flipped;

	Queue * sender_waiting_processes;

	Queue * receiver_buffer;
};

void callProcess(struct process * process);

int isBlocked(struct process * process);

#endif