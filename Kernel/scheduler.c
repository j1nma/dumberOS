#include "scheduler.h"
#include <drivers.h>
#include "process.h"
#include "stack.h"
#include "interruptions.h"
#include "asynchronousMP.h"


static struct scheduler * scheduler;

typedef int (*EntryPoint)();

// static struct process * mainProcess;
// static struct process * processes[10];

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

	if (scheduler->current->process->flipped == 15) {
		scheduler->current->process->kernelStack = esp;
		return scheduler->current->process->userStack;
	} else {
		scheduler->current->process->userStack = esp;
		return scheduler->current->process->kernelStack;
	}

}


void * switchKernelToUser() {
	if (scheduler->current->process->flipped == 15) {
		return scheduler->current->process->kernelStack;
	} else {
		return scheduler->current->process->userStack;
	}
}

void * switchStack(void * from_rsp, void * to_rsp) {

}

void * swap(void * from_rsp, void * to_rsp) {

}

void unflip() {
	scheduler->current->process->flipped = 0;
}

void next() {
	scheduler->current = scheduler->current->next;
}

void schedule() {

	if (isEmpty()) {
		next();
		while (scheduler->current->process->state != RUNNING)
			next();
	} else {

		struct process * process1 = pop();

		addProcess(process1); //Lo pongo como next.
		scheduler->current = scheduler->current->next; //Paso al next, lo pongo como current.
		callProcess(process1);
	}
}

void startProcess(struct process * process) {

	process->state = RUNNING;

	addProcess(process); //Lo pongo como next.

	scheduler->current = scheduler->current->next; //Paso al next, lo pongo como current.

	enableTickInter(); // Lo hago aca, porque es posible que el primer tick, entre antes que hayan procesos en el scheduler.

	callProcess(process);
}

void queueProcess(struct process * process) {

	push(process);
}

void addProcess(struct process * process) {

	struct process_node * newNode;
	newNode = (struct process_node *)malloc(sizeof(struct process_node));

	newNode->process = process; //Le asigno el proceso
	newNode->process->pid = pid; //Le asigno el pid

	if (pid == 0) { //Es el primero?
		scheduler->current = newNode;
		scheduler->current->next = newNode; //Lo pongo como current y el que le sigue a current
	} else {
		newNode->next = scheduler->current->next;
		scheduler->current->next = newNode; //Lo pongo como el siguiente al current y cambio las referencias del siguiente previo.
	}
	pid++;
}

void unblock(int code) {
	struct process_node * current = scheduler->current;

	for (int i = 0; i < pid; i++) {
		if (current->process->state == code) {
			current->process->state = RUNNING;
			break;
		} else {
			current = current->next;
		}
	}
}

void blockCurrent(int code) {

	scheduler->current->process->state = code;
	scheduler->current->process->flipped = 15;
	// enableTickInter();
	// endInter();
	int20();
	// schedule();
	// irq0Handler();
}

struct process * getProcess(int get_pid) {
	struct process_node * current = scheduler->current;

	for (int i = 0; i < pid; i++) {
		if (current->process->pid == get_pid) {
			return current->process;
		} else {
			current = current->next;
		}
	}

	return NULL;
}


/* start IPC */

int isBlocked(struct process * process) {
	return process->state == MESSAGE_BLOCK;
}

void blockProcess(struct process * process) {
	process->state = MESSAGE_BLOCK;

	if (process->pid) write("Blocking 1.\n", 13);

	if (process->pid == 0) write("Blocking 0.\n", 13);

	if (process->pid == 2) write("Blocking 2.\n", 13);

	mutex_up();

	int20();
}

void awakeProcess(int awake_pid) {
	struct process_node * current = scheduler->current;

	if (awake_pid == 0) write("Trying to awake 0.\n", 20);

	if (awake_pid == 1) write("Trying to awake 1.\n", 20);

	if (awake_pid == 2) write("Trying to awake 2.\n", 20);

	int i;
	for (i = 0; i < pid; i++) {
		if (current->process->pid == awake_pid) {
			current->process->state = RUNNING;
			// current->process->state = MESSAGE_UNBLOCK;

			write("Awoke somebody.\n", 17);
			return;
		} else {
			current = current->next;
		}
	}

	if (i == pid) write("Couldn't awake anybody.\n", 25);

}

/* end IPC */

