#ifndef PROCESS_H
#define PROCESS_H

// https://unix.stackexchange.com/questions/80038/what-is-the-structure-of-a-linux-process

struct process {

	// pid_t pid

	// TASK_RUNNING, TASK_INTERRUPTIBLE, TASK_UNINTERRUPTIBLE, TASK_STOPPED, TASK_ZOMBIE and TASK_DEAD

	int pid;

	int state;

	int priority;

	void * userStack;

	void * kernelStack;

	void * entryPoint;

	// prio_array_t *array
};


#endif