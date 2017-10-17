#ifndef PROCESS_H
#define PROCESS_H

// https://unix.stackexchange.com/questions/80038/what-is-the-structure-of-a-linux-process

struct process {

	// pid_t pid
	int pid;

	int state;

	int priority;

	void * userStack;

	void * kernelStack;

	void * entryPoint;

	// prio_array_t *array
};

#endif