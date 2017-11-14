#include "tests.h"

void initWelcome();
static int exit = FALSE;


void process2() {
	printf("I am process: %d waiting for message.\n", getPid());
	char * re = receive();
	printf("I am: %d, ", getPid());
	printf("recieved message: %s \n", re);

	LoopNop();
}

void processShell() {
	int lastPID = 0;
	for (int i = 0; i < 10; ++i) {
		printf("I am process: %d\n", getPid());
		int pid = createProcess(&process2);
		printf("Created process: %d\n", pid);
		char msg[20];
		scanf("%s", &msg);
		lastPID = pid;
	}

	LoopNop();
}

void processPrompt() {
	// Loop shell until user exits OS
	while (!exit) {
		char f[100];
		printf("dummyOS $ ");
		scanf("%s", &f);
		char** params = str_split(f);
		exit = runCommand(parseCommand(params[0]), params);
	}

	// LoopNop();
}

void processIdle() {
	LoopNop();
}

void memoryError(int error) {
	switch (error) {
		case 1: {
			printf("Error: Value not stored correctlly on page.\n");
			break;
		}
		case 2: {
			printf("Error: Value not retained on page after free. Not critical.\n");
			break;
		}
		case 3: {
			printf("Error: Alloc didn't return the same page. Shame.\n");
			break;
		}
		default: {
			printf("Error: %d\n", error);
			break;
		}
	}
}

void testMemory() {
	
	printf("Allocating 1 page.\n");
	char *page = alloc(1 * sizeof(char));

	
	printf("Writing all a's.\n");
	for (int i = 0; i < 1024*4*sizeof(char); ++i) {
		page[i] = 'a';
	}

	if (page[1024*4*sizeof(char) -1] == 'a')
		printf("On the last byte of the page, value: %c.\n", page[1024*4*sizeof(char) -1]);
	else{
		memoryError(1);
		freeP(page);
		return;
	}


	printf("Freeing page.\n");
	freeP(page);

	if (page[1024*4*sizeof(char) -1] == 'a')
		printf("The value is the same: %c. Not overwriten.\n", page[1024*4*sizeof(char) -1]);
	else {
		memoryError(2);
	}


	printf("Allocating new page, should be on the same space. (Multi-processing not contemplated)\n");
	char *page2 = alloc(1 * sizeof(char));

	printf("Writing all b's.\n");
	for (int i = 0; i < 1024*4*sizeof(char); ++i) {
		page2[i] = 'b';
	}


	if (page[1024*4*sizeof(char) -1] == 'b')
		printf("The value in the first pointer should be: %c because they are in the same space.\n", page[1024*4*sizeof(char) -1]);
	else {
		memoryError(3);
	}

	printf("Freeing second page.\n");
	freeP(page2);

}

int main() {
	

	initWelcome();

	int pid = createProcess(&processIdle);

	processPrompt();

	killProcess(pid);
	printf("See ya! :D");
	halt();

	return 0;
}

void iam() {
	int k = 0;
	while (1) {
		if (k % 500000000 == 0) {
			printf("I am process: %d... looping...\n", getPid());
		}
		k++;
	}
}

/**
Welcoming sign of the OS
*/
void initWelcome () {
	runCommand(COMMAND_CLEAR, "null");
	printf(" _____                                   ____   _____ \n");
	printf("|  __ \\                                 / __ \\ / ____|\n");
	printf("| |  | |_   _ _ __ ___  _ __ ___  _   _| |  | | (___  \n");
	printf("| |  | | | | | '_ ` _ \\| '_ ` _ \\| | | | |  | |\\___ \\ \n");
	printf("| |__| | |_| | | | | | | | | | | | |_| | |__| |____) |\n");
	printf("|_____/ \\__,_|_| |_| |_|_| |_| |_|\\__, |\\____/|_____/ \n");
	printf("                                   __/ |              \n");
	printf("                                  |___/               \n");
	printf("\n\n\n");
	return;
}
