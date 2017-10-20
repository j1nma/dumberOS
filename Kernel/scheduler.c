#include "scheduler.h"

static struct scheduler * scheduler;

// static struct process * mainProcess;
// static struct process * processes[10];

static int pid = 0;


void * switchUserToKernel(void * esp) {
	// ncPrintChar(esp);
	scheduler->current->process->userStack = esp;
	// ncPrintChar(mainProcess->userStack);
	return scheduler->current->process->kernelStack;
}


void * switchKernelToUser() {
	// ncPrintChar(mainProcess->userStack);

	//  scheduler->schedule();

	return scheduler->current->process->userStack;
}

void * switch_stack(void * from_rsp, void * to_rsp) { //Cambiar a camelcase

}

void * swap(void * from_rsp, void * to_rsp) {

}

void * schedule() {

	// TODO: mandar el current a null??

	scheduler->current = scheduler->current->next;

}

void addProcess(struct process * process) {

	process->pid = pid;
	pid++;

	struct process_node * newNode;
	newNode->process = process;

	// if (scheduler->current == nullptr) { // TODO: NO LO HACE BIEN, CHEQUEAR NULLPTR

		scheduler->current = newNode;
		scheduler->current->next = newNode;

	// } else {

		struct process_node * next = scheduler->current->next;
		scheduler->current->next = newNode;
		newNode->next = next;

	// }

	// scheduler->current = scheduler->first; //Esto hay que pensarlo.
}

// Copyright (c) 2015, Rodrigo Rearden

//TODO: ver que funcione como esperado
/*void removeProcess(struct process * process) {
	struct process_node * previous = scheduler->current;
	struct process_node * nodeToRemove = scheduler->current->next;

	if (scheduler->current == nullptr) {
		return;
	} else if (previous == nodeToRemove && process == current->process) {
		// delete current;
		scheduler->current = nullptr;
		// TODO: aborta si saco el current?
		return;
	}

	while(nodeToRemove->process != process) {
		previous = nodeToRemove;
		nodeToRemove = nodeToRemove->next;
	}

	previous->next = nodeToRemove->next;
	scheduler->current = nullptr;
	// delete nodeToRemove;
}*/

void blockMain() {
	//mainProcess->state = 1;
}

void unblockMain() {
	// mainProcess->state = 0;
}



