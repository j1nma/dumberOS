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

};

void * schedule();

void addProcess(struct process * process);

void removeProcess(struct process * process);

void * fillStackFrame(void * entryPoint, void * userStack);

void blockMain();

void unblockMain();

#endif