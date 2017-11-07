#ifndef _PROCESS_H
#define _PROCESS_H

#include "queue.h"

#define MESSAGE_SIZE 20

// https://unix.stackexchange.com/questions/80038/what-is-the-structure-of-a-linux-process

struct process {

	int pid;

	int state;

	int priority;

	void * userStack;

	void * kernelStack;

	void * flippedStack;

	void * entryPoint;

	int flipped;

	Queue * sender_waiting_processes;

	Queue * receiver_buffer;
};

void callProcess(struct process * process);

int isBlocked(struct process * process);

#endif