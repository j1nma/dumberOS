#include "scheduler.h"

static struct scheduler * scheduler;

void * switchUserToKernel(void * esp) {

	// switch_stack(current, current.kernel);

	// schedule();
	
	// switch_stack(current.kernel,current);

}


void * switchKernelToUser() {


}

void * switch_stack(void * from_rsp, void * to_rsp) {

}

void * swap(void * from_rsp, void * to_rsp) {

}

void * schedule() {

}