#include "tests.h"

int lastPID;

void processSenderReceiver() {

	if (getPid() % 2 == 0) {
		// Receiver
		printf("Hi! I am process %d waiting for message.\n", getPid());
		char * msg = receive();
		printf("I am process %d, ", getPid());
		printf("recieved message: %s\n", msg);

	} else {
		// Sender
		printf("Hi! I am process %d sending message.\n", getPid());
		char * msg = "Hi Dave. I'm HAL.\n";
		send(msg, lastPID);
	}

	LoopNop();

}

void processMessagePassing() {

	lastPID = getPid();

	int pid;

	for (int i = 0; i < 2; i++) {
		printf("I am process %d\n", getPid());
		lastPID = pid = createProcess(&processSenderReceiver);
	}

	for (int i = 0; i < 2; i++) {
		killProcess(pid--);
	}

}