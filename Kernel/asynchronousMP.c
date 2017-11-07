#include "asynchronousMP.h"
#include "message.h"
#include "mutex.h"
#include "process.h"
#include "queue.h"
#include "scheduler.h"

#include "drivers.h"
#include "interruptions.h"

void disableTaskSwitch();

void disableTaskSwitch() {
	setPicMaster(0x01);
	setPicSlave(0x00);
}

void enableTaskSwitch() {
	enableTickInter();
}


void mutex_down() {
	down();
}

void mutex_up() {
	up();
}

int peekWaitQueue(Queue * sender_waiting_processes) {
	int ans = malloc(sizeof(int));

	if (queuePeek(sender_waiting_processes, &ans) == 0) {
		free(ans);
		return 1;
	}

	return -1;
}

void pushWaitQueue(Queue * sender_waiting_processes, int sender_pid) {
	if (!enqueue(sender_waiting_processes, &sender_pid)) {
		write("Enqueued\n", 10);
	} else {
		write("NOT Enqueued\n", 14);
	}

}


int popWaitQueue(Queue * sender_waiting_processes) {
	int ans = malloc(sizeof(int));

	dequeue(sender_waiting_processes, &ans);

	int aux = ans;

	free(ans);

	return aux;
}

/*
Now let's start with sending a message, and not care about. This could lead to losing a message,
which we can't afford, so we'll do a trick here.
Despite of being asynchronous, we will block if receiver buffer is full,
and we'll continue only after there's space for our message.
*/
void asyncSend(char * message, int destination_pid) {

	// disableTaskSwitch();
	mutex_down();

	struct process * destination = getProcess(destination_pid);

	Queue * tmpbuff = destination->receiver_buffer;

	if (!enqueue(tmpbuff, message)) write("Encole mi mensaje.\n", 20);

	if (isBlocked(destination)) awakeProcess(destination->pid);

	mutex_up();


	// enableTaskSwitch();
}

/*
Doesn't matter whether it's synchronized or not, receiver must block if it's message queue
is empty and there's nothing to process.
*/
char * asyncReceive() {

	struct process * current_process = getCurrentProcess();

	char * tmp = malloc(MESSAGE_SIZE * sizeof(char));

	// disableTaskSwitch();
	mutex_down();

	Queue * rb = current_process->receiver_buffer;

	if (rb->sizeOfQueue == 0) {
		write("Tengo 0 msgs.\n", 15);
	}

	if (rb->sizeOfQueue == 1) {
		write("Tengo 1 msgs.\n", 15);
	}

	if (rb->sizeOfQueue == 2) {
		write("Tengo 2 msgs.\n", 15);
	}

	if (rb->sizeOfQueue == 0) {
		write("Blocking...\n", 13);
		blockProcess(current_process);
		// blockCurrent(MESSAGE_BLOCK);
	}

	dequeue(rb, tmp);

	Queue * swp = current_process->sender_waiting_processes;

	while (getQueueSize(swp)) { awakeProcess(popWaitQueue(swp)); }

	free(tmp);

	mutex_up();
	// enableTaskSwitch();

	return tmp;
}