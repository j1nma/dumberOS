#include "asynchronousMP.h"
#include "process.h"
#include "queue.h"
#include "scheduler.h"

#include "drivers.h"
#include "lib.h"
#include "interruptions.h"
#include "mutualExclusion.h"

int * bolt;

void initMutex() {
	bolt = newBolt();
}

void deleteMutex() {
	deleteBolt(bolt);
}

int * getBolt() {
	return bolt;
}

/*
If destination process does not exist, the operation will terminate. If it does, the message is added to the receiver's
queue of messages. If it's blocked by message passing, it is awaken. Mutex implemented over the bolt.
*/
void asyncSend(char * message, int destination_pid) {

	acquireBolt(bolt);

	struct process * destination;

	if (getProcess(destination_pid, &destination) == 0) {
		write("Receiver process does not exist.\n", 34);
		releaseBolt(bolt);
		return;
	}

	Queue * tmpbuff = destination->receiver_buffer;

	if (!enqueue(tmpbuff, message)) {

		// write("Encole mi mensaje.\n", 20);

		if (isBlocked(destination)) {
			unblockProcess(destination);
		}
	}

	releaseBolt(bolt);
}

/*
Receiver process blocks if it's message queue is empty and there's nothing to process.
Mutex implemented over the bolt.
*/
char * asyncReceive() {

	acquireBolt(bolt);

	struct process * current_process = getCurrentProcess();

	char * tmp = memAlloc((MESSAGE_SIZE + 1) * sizeof(char));

	Queue * rb = current_process->receiver_buffer;

	if (rb->sizeOfQueue == 0) {
		// write("Blocking...\n", 13);
		releaseBolt(bolt);
		blockCurrent(MESSAGE_BLOCK);
	}

	dequeue(rb, tmp);

	memFree(tmp);

	releaseBolt(bolt);

	return tmp;
}