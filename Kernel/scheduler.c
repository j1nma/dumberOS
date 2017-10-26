#include "scheduler.h"
#include <drivers.h>
#include "process.h"
#include "stack.h"

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

int getCurrentSP() {
	return scheduler->current->process->userStack;
}


void * switchUserToKernel(void * esp) {

	scheduler->current->process->userStack = esp;
	return scheduler->current->process->kernelStack;
}


void * switchKernelToUser() {
	
	return scheduler->current->process->userStack;
}

void * switch_stack(void * from_rsp, void * to_rsp) { //Cambiar a camelcase

}

void * swap(void * from_rsp, void * to_rsp) {

}

void schedule() {

	if (isempty()) {
		scheduler->current = scheduler->current->next;
		while (scheduler->current->process->state != RUNNING)
			scheduler->current = scheduler->current->next;
	}else{
	
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
	
	enableTickInter(); // Lo hago aca, porque es posible que el primer tick, entre antes que hayan procesos en el schedueler.

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
	}else{	
		newNode->next = scheduler->current->next;
		scheduler->current->next = newNode; //Lo pongo como el siguiente al current y cambio las referencias del siguiente previo.
	}
	pid++; //Aumento el pid
}

void unblock(int code) {
	struct process_node *current = scheduler->current;
	for (int i = 0; i < pid; i++) {
		if (current->process->state == code) {
			void * aux = current->process->userStack;
			current->process->userStack = current->process->kernelStack;
			current->process->kernelStack = aux;
			current->process->state = RUNNING;
			break;
		}else{
			current = current->next;
		}
	}
}

void blockCurrent(int code) {
	void * aux = scheduler->current->process->userStack;
	scheduler->current->process->userStack = scheduler->current->process->kernelStack;
	scheduler->current->process->kernelStack = aux;
	scheduler->current->process->state = KEYBOARD_BLOCK;
	enableTickInter();
	endInter();
	// int20();
	schedule();
}


