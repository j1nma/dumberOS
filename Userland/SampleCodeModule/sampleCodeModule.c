#include <libc.h>
#include <commands.h>

void initWelcome ();
static int exit = FALSE;

int main() {

	// OS welcome
	// initWelcome();


	int pid = getPid();
	printf("I am process: %d\n", getPid());

	if (pid != 0) {
		// for (int i = 0; i < 1000000000 * pid; i++);

		printf("I am process: %d, sending...\n", getPid());

		char * msg;
		if (pid == 1) {
			msg = "DESDE 1 CON AMOR\0";
		}else if (pid == 2) {
			msg = "DESDE 2 CON AMOR\0";
		}

		send(msg, pid - 1);

		printf("Sali del send.\n");

	} else {

		// int k = 0;
		// while (1) {
		// if (k % 100000000 == 0) {

		printf("I am process: %d, receiving...\n", getPid());

		printf("RECEIVED MSG n1: %s \n", receive());

		printf("RECEIVED MSG n2: %s \n", receive());

	}

	iam();
	// 	// k++;
	// 	// }

	// } else {
	// 	iam();
	// }

	// char f[100];
	// if (getPid() != 0) {
	// 	int pid = getPid();
	// 	// for (int i = 0; i < pid + 1; i++) {
	// 	printf("Soy el proceso: %d, espero teclado...\n", getPid());
	// 	scanf("%s", &f);
	// 	printf("Soy: %d, volvi con: %s!!!\n", getPid(), f);
	// 	// }
	// 	iam();
	// } else {
	// 	iam();
	// }	

	// else {
	// 	printf("Soy el proceso: %d\n", getPid());
	// }

	// Loop shell until user exits OS
	// while(!exit){
	// 	char f[100];
	// 	printf("dummyOS $ ");
	// 	scanf("%s", &f);
	// 	char** params = str_split(f);
	// 	exit = runCommand(parseCommand(params[0]), params);
	// }

	// printf("bye! :(\n");
	// halt();

	return 0;
}

void iam() {
	int k = 0;
	while (1) {
		if (k % 500000000 == 0) {
			printf("Soy el proceso: %d!!!\n", getPid());
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
