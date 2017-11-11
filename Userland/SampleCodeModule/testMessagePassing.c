#include "tests.h"

void processSender() {

	int pid = getPid();

	printf("Hi! I am process %d sending message to %d, \n", pid, pid-1);
	char * msg = "Hi Dave. I'm HAL.\n";
	send(msg, pid-1);

	LoopNop();

}

void processReceiver() {

	int pid = getPid();

	printf("Hi! I am process %d waiting for message.\n", pid);
	char * msg = receive();
	printf("I am process %d, ", pid);
	printf("recieved message: %s\n", msg);

	LoopNop();

}

void processMessagePassing() {

	int receiver;
	int sender;

	printf("I am process %d\n", getPid());

	receiver = createProcess(&processReceiver);
	sender = createProcess(&processSender);

	killProcess(sender);
	killProcess(receiver);

}