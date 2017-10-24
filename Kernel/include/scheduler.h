#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"
#include "process_node.h"

/* Context switching */

void * switchUserToKernel(void * esp);

void * switchKernelToUser();

void * switch_stack(void * from_rsp, void * to_rsp);

void * swap(void * from_rsp, void * to_rsp);

/* Scheduler */

struct scheduler {

	struct process_node * current;
	struct process_node * ready;

};

void schedule();

void queueProcess(struct process * process);

void forzeProcess(struct process * process);

void addProcess(struct process * process);

void removeProcess(struct process * process);

void * fillStackFrame(void * entryPoint, void * userStack);

void blockMain();

void unblockMain();

int getCurrentPid();

int getCurrentSP();

void initScheduler();

#endif