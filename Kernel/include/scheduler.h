#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"
#include "process_node.h"

#define KEYBOARD_BLOCK 4
#define MESSAGE_BLOCK 5
#define MESSAGE_UNBLOCK 6
#define RUNNING 0

/* Context switching */

void * switchUserToKernel(void * esp);

void * switchKernelToUser();

void * switchStack(void * from_rsp, void * to_rsp);

void * swap(void * from_rsp, void * to_rsp);

/* Scheduler */

struct scheduler {

	struct process_node * current;

};

void unflip();

void schedule();

void queueProcess(struct process * process);

void startProcess(struct process * process);

void addProcess(struct process * process);

void removeProcess(struct process * process);

void blockCurrent(int code);

void unblock(int code);

int getCurrentPid();

struct process * getCurrentProcess();

void * getCurrentSP();

void initScheduler();

struct process * getProcess(int get_pid);

struct process * getCurrentProcess();

/* IPC */

void blockProcess(struct process * process);

void awakeProcess(int pid);

#endif