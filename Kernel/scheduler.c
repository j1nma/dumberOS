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


	struct process_node * newNode; //Creo un node nuevo
	newNode = (struct process_node *)malloc(sizeof(struct process_node)); //Lo aloco

	newNode->process = process; //Le asigno el proceso
	newNode->process->pid = pid; //Le asigno el pid

	/* IPC */
	queueInit(newNode->process->sender_waiting_processes, sizeof(int));
	cbInit(newNode->process->receiver_buffer, MESSAGE_SIZE);
	/* IPC */

	if (pid == 0) { //Es el primero?
		scheduler->current = newNode;
		scheduler->current->next = newNode; //Lo pongo como current y el que le sigue a current
	} else {
		newNode->next = scheduler->current->next;
		scheduler->current->next = newNode; //Lo pongo como el siguiente al current y cambio las referencias del siguiente previo.
	}
	pid++; //Aumento el pid
}

void unblock(int code) {
	struct process_node *current = scheduler->current;
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

	scheduler->current->process->state = KEYBOARD_BLOCK;
	scheduler->current->process->flipped = 15;
	// enableTickInter();
	// endInter();
	int20();
	// schedule();
	// irq0Handler();
}







/* start IPC */

struct process * getProcess(int get_pid);


void blockProcess(struct process * process) {
	process->state = MESSAGE_BLOCK;
}

void awakeProcess(int awake_pid) {
	struct process_node * current = scheduler->current;

	for (int i = 0; i < pid; i++) {

		if (current->process->pid == awake_pid) {

			// current->process->state = RUNNING;

			current->process->state = MESSAGE_UNBLOCK;

			break;

		} else {

			current = current->next;

		}
	}

}

int isBlocked(struct process * process) {
	return process->state == MESSAGE_BLOCK;
}

int peekWaitQueue(struct process * receiver) {

	Queue * q = receiver->sender_waiting_processes;

	int ans = -1;
	queuePeek(q, &ans);

	return ans;
}

void pushWaitQueue(struct process * receiver, struct process * sender) {

	Queue * q = receiver->sender_waiting_processes;

	enqueue(q, &(sender->pid));
}


int popWaitQueue(struct process * process) {

	Queue * q = process->sender_waiting_processes;

	int ans;
	dequeue(q, &ans);

	return ans;
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
}

/*
Now let's start with sending a message, and not care about. This could lead to losing a message,
which we can't afford, so we'll do a trick here.
Despite of being asynchronous, we will block if receiver buffer is full,
and we'll continue only after there's space for our message.
*/
void asyncSend(char * message, int destination_pid) {

	disableTaskSwitch();

	struct process * destination = getProcess(destination_pid);

	circular_buffer * tmpbuff = destination->receiver_buffer; //temporarily map destination's buffer into sender process' address space

	if (tmpbuff->count == MAXITEMS) { //if receiver buffer is full, block
		pushWaitQueue(destination, scheduler->current->process); //record this process in dst's sender queue
		blockProcess(scheduler->current->process);
	}

	cbPushBack(tmpbuff, message);

	if (isBlocked(destination)) awakeProcess(destination->pid); //if destination process is blocked for receiving, awake it
	
	enableTaskSwitch();

}

/*
Doesn't matter whether it's synchronized or not,
receiver must block if it's message queue is empty,
and there's nothing to process.
*/
char * asyncRecieve() {

	struct process * current_process = scheduler->current->process;

	struct message * tmp;

	disableTaskSwitch();

	circular_buffer * cb = current_process->receiver_buffer;

	if (cb->count == 0) blockProcess(current_process); //if there's nothing to get, block

	cbPopFront(cb, tmp);

	while (peekWaitQueue(current_process) != -1) awakeProcess(popWaitQueue(current_process)); //awake blocked processes waiting to send

	enableTaskSwitch();

	return tmp->message;
}
/* end IPC */

