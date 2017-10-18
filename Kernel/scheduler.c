#include "scheduler.h"

static struct scheduler * scheduler;

static struct process * mainProcess;


void * switchUserToKernel(void * esp) {
	// ncPrintChar(esp);
	mainProcess->userStack = esp;
	// ncPrintChar(mainProcess->userStack);
	return mainProcess->kernelStack;
}


void * switchKernelToUser() {
	// ncPrintChar(mainProcess->userStack);
	return mainProcess->userStack;
}

void * switch_stack(void * from_rsp, void * to_rsp) { //Cambiar a camelcase

}

void * swap(void * from_rsp, void * to_rsp) {

}

void * schedule() {

}

void addProcess(struct process * process) {
	mainProcess = process;
}




