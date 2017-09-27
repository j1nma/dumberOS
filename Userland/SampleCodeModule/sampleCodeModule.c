/* sampleCodeModule.c */
#include <libc.h>
#include <commands.h>

void initWelcome ();
static int exit = FALSE;

int main() {
	

	
	// OS welcome
	initWelcome();
	
	// Loop shell until user exits OS
	while(!exit){
		char f[100];
		printf("dummyOS $ ");
		scanf("%s", &f);
		char** params = str_split(f);
		exit = runCommand(parseCommand(params[0]), params);
	}

	printf("bye! :(\n");
	halt();

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
