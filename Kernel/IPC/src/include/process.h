#ifndef _PROCESS_H
#define _PROCESS_H

#include "circular_buffer.h"
#include "queue.h"

#define RUNNING 0
#define MESSAGE_BLOCK 4
#define MESSAGE_UNBLOCK 5

// https://unix.stackexchange.com/questions/80038/what-is-the-structure-of-a-linux-process

struct process {

	// TASK_RUNNING, TASK_INTERRUPTIBLE, TASK_UNINTERRUPTIBLE, TASK_STOPPED, TASK_ZOMBIE and TASK_DEAD

	int pid;

	int state;

	int priority;

	void * userStack;

	void * kernelStack;

	void * entryPoint;

	int flipped;

	Queue sender_waiting_processes; // pids

	circular_buffer * receiver_buffer;

};

#endif