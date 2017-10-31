#include "process.h"
#include "queue.h"
// #include <drivers.h>

// void callProcess(struct process * process) {
// 	endInter();
// 	switchStackAndJump((process->userStack), (process->entryPoint));
// }

int topWaitQueue(struct process * process) {
	Queue q = process->blocked_waiting_processes;

	int ans;

	queuePeek(&q, &ans);

	return ans;
}

int popWaitQueue(struct process * process) {
	Queue q = process->blocked_waiting_processes;

	int ans;

	dequeue(&q, &ans);

	return ans;
}