#include "libc.h"
#include "commands.h"
#include "tests.h"

int lastPID;

void processSenderReceiver() {

	if (getPid() == lastPID + 1) {
		// Receiver
		printf("Hi! I am process %d waiting for message.\n", getPid());
		char * msg = receive();
		printf("I am process %d, ", getPid());
		printf("recieved message: %s\n", msg);

	} else {
		// Sender
		printf("Hi! I am process %d sending message.\n", getPid());
		char * msg = "Hi Dave. I'm HAL.\n";
		send(msg, 2);
	}

	LoopNop();

}

void processMessagePassing() {

	lastPID = getPid();

	for (int i = 0; i < 2; i++) {
		printf("I am process %d\n", getPid());
		int pid = createProcess(&processSenderReceiver);
		printf("Process %d was created.\n", pid);
	}

	LoopNop();

}