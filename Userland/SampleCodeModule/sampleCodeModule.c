#include <libc.h>
#include <commands.h>



void initWelcome ();
static int exit = FALSE;


void process2 (){
	printf("I am process: %d waiting for message.\n", getPid());
	char * re = receive();
	printf("I am: %d, ", getPid());
	printf("recieved message: %s \n", re);
	
	LoopNop();
}

void processShell (){
	int lastPID = 0;
	for (int i = 0; i < 10; ++i)
	{
		printf("I am process: %d\n", getPid());
		int pid = createProcess(&process2);
		printf("Created process: %d\n", pid);
		char msg[20];
		scanf("%s", &msg);
		lastPID = pid;
	}
	// for (int i = 0; i < lastPID + 1; ++i)
	// {
	// 	char * msg = "msg";
	// 	send(msg, i);
	// }
	LoopNop();
}

int main() {

	int i = createProcess(&processShell);
	
	LoopNop();

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
