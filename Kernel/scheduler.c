#include "scheduler.h"
#include <drivers.h>
#include "process.h"

static struct scheduler * scheduler;

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

void schedule() {

	// TODO: mandar el current a null??

	scheduler->current = scheduler->current->next;

}
void addProcess(struct process * process) {


	// process->pid = pid;
	// ncPrintHex(process->pid);
	// ncPrintHex(pid);
	

	struct process_node * newNode;
	// newNode = (struct process_node *)malloc(sizeof(struct process_node)); 

	newNode->process = process;
	newNode->process->pid = pid;



	ncPrint("newNode->process->userStack: ");ncPrintHex(newNode->process->userStack);
	ncNewline();
	ncPrint("newNode->process->kernelStack: ");ncPrintHex(newNode->process->kernelStack);
	ncNewline();


	if (pid == 0) {
		scheduler->current = newNode;
		scheduler->current->next = newNode;
		ncPrint("First: ");ncPrintHex(scheduler->current);
		ncNewline();

		// ncPrint("scheduler->current->process->userStack: ");ncPrintHex(scheduler->current->process->userStack);
		// ncNewline();
		// ncPrint("scheduler->current->process->kernelStack: ");ncPrintHex(scheduler->current->process->kernelStack);
		// ncNewline();

	}else{

		newNode->next = scheduler->current->next;
		scheduler->current->next = newNode;

		ncPrint("New: ");ncPrintHex(newNode);
		ncNewline();

		ncPrint("Current: ");ncPrintHex(scheduler->current);
		ncNewline();
		
		ncPrint("Current->Next: ");ncPrintHex(scheduler->current->next);
		ncNewline();

		ncPrint("Current->Next->Next: ");ncPrintHex(scheduler->current->next->next);
		ncNewline();



		ncPrint("Current->pid: ");ncPrintHex(scheduler->current->process->pid);
		ncNewline();
		ncPrint("Current->next->pid: ");ncPrintHex(scheduler->current->next->process->pid);
		ncNewline();
		// ncPrint("Current->next->next->pid: ");ncPrintHex(scheduler->current->next->next->process->pid);
		// ncNewline();

	}

	pid++;
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



