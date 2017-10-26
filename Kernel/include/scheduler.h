#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"
#include "process_node.h"

#define KEYBOARD_BLOCK 4
#define RUNNING 0

/* Context switching */

void * switchUserToKernel(void * esp);

void * switchKernelToUser();

void * switch_stack(void * from_rsp, void * to_rsp);

void * swap(void * from_rsp, void * to_rsp);

/* Scheduler */

struct scheduler {

	struct process_node * current;

};

void schedule();

void queueProcess(struct process * process);

void startProcess(struct process * process);

void addProcess(struct process * process);

void removeProcess(struct process * process);

void * fillStackFrame(void * entryPoint, void * userStack);

void blockCurrent(int code);

void unblock(int code);

int getCurrentPid();

int getCurrentSP();

void initScheduler();

#endif