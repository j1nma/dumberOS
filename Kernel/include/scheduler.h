#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"
#include "process_node.h"

#define KEYBOARD_BLOCK 4
#define MESSAGE_BLOCK 5
#define CREATE_PROCESS_BLOCK 10
#define MESSAGE_UNBLOCK 6
#define BOLT_BLOCK 7
#define RUNNING 0

/* Context switching */

void * switchUserToKernel(void * esp);

void * switchKernelToUser(void * esp);

/* Scheduler */

struct scheduler {

	struct process_node * current;
	struct process_node * last_serviced;

};

void next();

void flip();

void unflip();

void schedule();

int createPid();

int killProcess(int pid);

void listProcesses();

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

int getProcess(int get_pid, struct process ** ret);

struct process * getCurrentProcess();

#endif