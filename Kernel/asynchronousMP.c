#include "asynchronousMP.h"
#include "mutex.h"
#include "process.h"
#include "MP_queue.h"
#include "scheduler.h"

#include "drivers.h"
#include "lib.h"
#include "interruptions.h"


void mutex_down() {
	down();
}

void mutex_up() {
	up();
}

/*
Now let's start with sending a message, and not care about. This could lead to losing a message,
which we can't afford, so we'll do a trick here.
Despite of being asynchronous, we will block if receiver buffer is full,
and we'll continue only after there's space for our message.
*/
void asyncSend(char * message, int destination_pid) {

	mutex_down();

	struct process * destination;

	if (getProcess(destination_pid, &destination) == 0) {
		mutex_up();
		return;
	}

	Queue * tmpbuff = destination->receiver_buffer;

	if (!enqueue(tmpbuff, message)) {

		// write("Encole mi mensaje.\n", 20);

		if (isBlocked(destination)) {
			unblockProcess(destination);
		}
	}

	mutex_up();

}

/*
Doesn't matter whether it's synchronized or not, receiver must block if it's message queue
is empty and there's nothing to process.
*/
char * asyncReceive() {

	mutex_down();

	struct process * current_process = getCurrentProcess();

	char * tmp = malloc((MESSAGE_SIZE + 1) * sizeof(char));

	Queue * rb = current_process->receiver_buffer;

	if (rb->sizeOfQueue == 0) {
		// write("Blocking...\n", 13);

		mutex_up();
		blockCurrent(MESSAGE_BLOCK);
	}

	dequeue(rb, tmp);

	free(tmp);

	mutex_up();

	return tmp;
}