#include "asynchronousMP.h"
#include "message.h"
#include "mutex.h"
#include "process.h"
#include "circular_buffer.h"
#include "queue.h"
#include "scheduler.h"

void mutex_down() {
	down();
}

void mutex_up() {
	up();
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

/*
Now let's start with sending a message, and not care about. This could lead to losing a message,
which we can't afford, so we'll do a trick here.
Despite of being asynchronous, we will block if receiver buffer is full,
and we'll continue only after there's space for our message.
*/
void asyncSend(char * message, int destination_pid) {

	mutex_down();

	struct process * destination = getProcess(destination_pid);

	circular_buffer * tmpbuff = destination->receiver_buffer; //temporarily map destination's buffer into sender process' address space

	if (tmpbuff->count == MAXITEMS) { //if receiver buffer is full, block

		struct process * current = getCurrentProcess();

		pushWaitQueue(destination, current); //record this process in dst's sender queue

		blockProcess(current);
		// blockCurrent(MESSAGE_BLOCK);
	}

	cbPushBack(tmpbuff, message);

	if (isBlocked(destination)) {
		awakeProcess(destination->pid); //if destination process is blocked for receiving, awake it
	}

	mutex_up();
}

/*
Doesn't matter whether it's synchronized or not, receiver must block if it's message queue
is empty and there's nothing to process.
*/
char * asyncReceive() {

	struct process * current_process = getCurrentProcess();

	char * tmp = malloc(sizeof(char) * MESSAGE_SIZE);

	mutex_down();

	circular_buffer * cb = current_process->receiver_buffer;

	if (cb->count == 0) {
		blockProcess(current_process); //if there's nothing to get, block
		// blockCurrent(MESSAGE_BLOCK);
	}

	cbPopFront(cb, tmp);

	while (peekWaitQueue(current_process) != -1) {
		awakeProcess(popWaitQueue(current_process)); //awake blocked processes waiting to send
	}

	mutex_up();

	return tmp;
}