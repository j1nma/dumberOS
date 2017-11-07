#include <libc.h>
#include <commands.h>

void initWelcome ();
static int exit = FALSE;

int main() {


	// OS welcome
	// initWelcome();
	// char f[100];
	// if (getPid() != 0) {
	// 	while (1) {
	// 		printf("Soy el proceso: %d, espero teclado...\n", getPid());
	// 		scanf("%s", &f);
	// 		printf("Volvi con: %s!!!\n", f);
	// 	}
	// } else {

	// 	int k = 0;
	// 	while (1) {
	// 		if (k % 200000000 == 0) {
	// 			printf("Soy el proceso: %d!!!\n", getPid());
	// 		}
	// 		k++;
	// 	}
	// }


	// OS welcome
	// initWelcome();

	if (getPid() == 1) {

		while (1) {
			printf("I am process: %d, sending...\n", getPid());

			char * msg = "DESDE 1 CON AMOR\0";

			send(msg, 0);

			printf("SENT!\n");
		}

	} else if (getPid() == 0) {

		int k = 0;
		while (1) {
			if (k % 100000000 == 0) {

				printf("I am process: %d, receiving...\n", getPid());

				printf("RECEIVED: %s \n", receive());

			}
			k++;
		}
	}

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
