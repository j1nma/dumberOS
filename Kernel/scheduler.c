#include "scheduler.h"
#include <drivers.h>
#include "process.h"
#include "stack.h"
#include "interruptions.h"
#include <naiveConsole.h>
#include "MP_queue.h"
#include "asynchronousMP.h"

#define FLIPPED 5

static struct scheduler * scheduler;

typedef int (*EntryPoint)();

static int pid = 0;

void initScheduler() {
	scheduler = (struct scheduler *)malloc(sizeof(struct scheduler));
}

int getCurrentPid() {
	return scheduler->current->process->pid;
}

struct process * getCurrentProcess() {
	return scheduler->current->process;
}

void * getCurrentSP() {
	return scheduler->current->process->userStack;
}


void * switchUserToKernel(void * esp) {

	if (scheduler->current->process->flipped != FLIPPED) { //Si no esta volteado, funciona normal.
		scheduler->current->process->userStack = esp;
		return scheduler->current->process->kernelStack;
	} else {
		scheduler->current->process->kernelStack = esp; //Si esta volteado, devolve el stack especial de flipped.
		return scheduler->current->process->flippedStack;
	}
}


void * switchKernelToUser(void * esp) {
	if (scheduler->current->process->flipped != FLIPPED) { //Si no esta volteado, funciona normal.
		scheduler->current->process->kernelStack = esp;
		return scheduler->current->process->userStack;
	} else {
		scheduler->current->process->flippedStack = esp; //Si esta volteado, guarda el SP del volteado y devolve el kernelStack.
		return scheduler->current->process->kernelStack;
	}
}

void flip() {
	scheduler->current->process->flipped = FLIPPED;
}

void unflip() {
	scheduler->current->process->flipped = 0;
}

void next() {
	scheduler->current = scheduler->current->next;
	while (scheduler->current->process->state != RUNNING) {
		scheduler->current = scheduler->current->next;
	}
}

void schedule() {

	if (isEmpty()) {
		next();
	} else {
		struct process * process1 = pop();
		addProcess(process1); //Lo pongo como next.
		next(); //Paso al next, lo pongo como current.
		unblockParent(process1->parent);
		callProcess(process1);
	}
}

void unblockParent(int pid) {
	struct process * parent;
	if (getProcess(pid, &parent) != 0) {
		if (parent->state == CREATE_PROCESS_BLOCK) {
			unblockProcess(parent);

		}
	}
}

int killProcess(int pid) {

	struct process_node * prev = scheduler->current;
	struct process_node * current = scheduler->current->next;

	for (int i = 0; i < pid; i++) {
		if (current->process->pid == pid) {

			prev->next = current->next;
			//TODO: free la memoria.

			return current->process->pid;
		} else {
			prev = prev->next;
			current = current->next;
		}
	}

	return 0;

}

void listProcesses() {

	struct process_node * current = scheduler->current;

	int prev;

	for (int i = 0; i < pid; i++) {
		struct process * p = current->process;

		if (prev > p->pid) break; // This is required due to the scheduler's circular list of processes.
		prev = i;

		write("[PID: ", 6);
		char pd[2];
		write(itoa(pd, p->pid), 2);

		write(", state: ", 9);
		char * state = stateToString(p->state);
		write(state, mystrlen(state));

		write(", parent: ", 10);
		char prnt[2];
		write(itoa(prnt, p->parent), 2);

		write("]\n", 3);

		current = current->next;
	}

}

void startProcess(struct process * process) {

	process->state = RUNNING;

	addProcess(process); //Lo pongo como next.

	next(); //Paso al next, lo pongo como current.

	enableTickInter(); // Lo hago aca, porque es posible que el primer tick, entre antes que hayan procesos en el scheduler.

	callProcess(process);
}

int createPid() {
	return pid++;
}

void queueProcess(struct process * process) {

	push(process);
}

void addProcess(struct process * process) {

	struct process_node * newNode;
	newNode = (struct process_node *)malloc(sizeof(struct process_node));

	newNode->process = process; //Le asigno el proceso

	if (process->pid == 0) { //Es el primero?
		scheduler->current = newNode;
		scheduler->current->next = newNode; //Lo pongo como current y el que le sigue a current
	} else {
		newNode->next = scheduler->current->next;
		scheduler->current->next = newNode; //Lo pongo como el siguiente al current y cambio las referencias del siguiente previo.
	}

	scheduler->last_serviced = newNode;
}

void unblock(int code) {

	struct process_node * current = scheduler->last_serviced;
	int startingPid = current->process->pid;
	current = current->next;

	while (current->process->state != code && current->process->pid != startingPid) {
		current = current->next;
	}
	current->process->state = RUNNING;
	scheduler->last_serviced = current;
}

void unblockProcess(struct process * sleeper) {
	sleeper->state = RUNNING;
}

void blockCurrent(int code) {

	flip();

	scheduler->current->process->state = code;

	int20();

	unflip();
}

int getProcess(int get_pid, struct process ** ret) {
	struct process_node * current = scheduler->current;

	for (int i = 0; i < pid; i++) {
		if (current->process->pid == get_pid) {
			*(ret) = current->process;
			return 1;
		} else {
			current = current->next;
		}
	}

	return 0;
}


/* start IPC */

int isBlocked(struct process * process) {
	return process->state != RUNNING;
}

/* end IPC */

