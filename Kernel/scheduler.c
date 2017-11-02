#include "scheduler.h"
#include <drivers.h>
#include "process.h"
#include "stack.h"
#include "interruptions.h"
#include "ioBlocked.h"
#include <naiveConsole.h>
#include "Queue.h"

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
	while (scheduler->current->process->state != RUNNING){
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
		callProcess(process1);
	}
}

void startProcess(struct process * process) {

	process->state = RUNNING;

	addProcess(process); //Lo pongo como next.

	next(); //Paso al next, lo pongo como current.

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

	struct process * ready;
	if(!isQueueEmpty()) {
		ready = queueRemove();
		int t = ready->pid;
		// ncPrint("Desencolo: ");ncPrintDec(t);
        ready->state = RUNNING;
    }
}

void blockCurrent(int code) {

	scheduler->current->process->state = KEYBOARD_BLOCK;
	int t = scheduler->current->process->pid;
	// ncPrint("Encolo: ");ncPrintDec(t);
	queueInsert(scheduler->current->process);

	enableTickInter();

	int20();
}


