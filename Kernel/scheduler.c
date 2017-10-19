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
	return scheduler->current->process->userStack;
}

void * switch_stack(void * from_rsp, void * to_rsp) { //Cambiar a camelcase

}

void * swap(void * from_rsp, void * to_rsp) {

}

void * schedule() {

}

void addProcess(struct process * process) {
	process->pid = pid;
	pid++;


	struct process_node * newNode;
	newNode->process = process; 
	scheduler->current = newNode;
	return;
	
	// struct process_node * aux = scheduler->first;

	// struct process_node * newNode;	
	// newNode->process = process;

	// scheduler->first = newNode;

	// if (!scheduler->last) {
	// 	scheduler->last = newNode;
	// }

	// scheduler->last->next = scheduler->first;

	// scheduler->first->next = aux;

	// scheduler->current = scheduler->first; //Esto hay que pensarlo.
}

void blockMain(){
	//mainProcess->state = 1;
}

void unblockMain(){
	// mainProcess->state = 0;
}



