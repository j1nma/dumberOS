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

	printf("Soy: %d", getPid());
	printf("Mi SP: %h\n", getRSP());
	
	// Loop shell until user exits OS
	char dup[250];
	while (!exit) {
		char f[250];
		for (int i = 0; i < 250; ++i)
			dup[i] = f[i];

		printf("dummyOS $ ");
		int t = scanf("%s", &f);

		if (t == 1) {
			for (int i = 0; i < 250; ++i)
				f[i] = dup[i];
			char** params = str_split(dup);
			exit = runCommand(parseCommand(params[0]), params, dup + strlength(params[0]) + 1);
		} else {
			char** params = str_split(f);
			exit = runCommand(parseCommand(params[0]), params, f + strlength(params[0]) + 1);
		}


	}
}

void processIdle() {
	LoopNop();
}

int main() {

	// initWelcome();

	processPrompt();

	printf("See ya! :D %d", getPid());
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
	runCommand(COMMAND_CLEAR, "null", "null");
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
