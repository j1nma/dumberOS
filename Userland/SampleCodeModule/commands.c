#include <manpages.h>
#include <commands.h>

/**
Convert string to command int value.

@param commandName Pointer to command string value
@return int Command int value
*/
int parseCommand(char *commandName) {
	
	if ( strcmp((char *)commandName, "echo") == 0 ) 
		return COMMAND_ECHO;	
	else if ( strcmp((char *)commandName, "time") == 0 ) 
		return COMMAND_TIME;
	else if ( strcmp((char *)commandName, "net") == 0 ) 
		return COMMAND_NET;
	else if ( strcmp((char *)commandName, "help") == 0 ) 
		return COMMAND_HELP;
	else if ( strcmp((char *)commandName, "man") == 0 ) 
		return COMMAND_MAN;
	else if ( strcmp((char *)commandName, "exit") == 0 ) 
		return COMMAND_POWEROFF;
	else if ( strcmp((char *)commandName, "clear") == 0 ) 
		return COMMAND_CLEAR;
	else
		return COMMAND_NULL;
	
}

char buf[1000];

/**
Run command with the submited params

@param command Int value of command to run
@param params Array of string params for command
@return int 0 or 1 if shell should exit
*/
int runCommand(int command, char **params) {

	switch (command) {
		case COMMAND_ECHO: {
			printf("%s ", (char *)params[1]);
			printf("%s ", (char *)params[2]);
			printf("%s ", (char *)params[3]);
			printf("%s\n", (char *)params[4]);
			
			break;
		}
		case COMMAND_TIME: {
			char *time = (char *)malloc(20);
			getTime(time);
			printf("Current time: %s\n", time);
			break;
		}
		case COMMAND_NET: {
			
			if (strcmp((char *)params[1], "send") == 0){
				
				printf("Enter mac (Format: FF:FF:FF:FF:FF:FF): ");
				scanf("%s", buf);
				if (strlength(buf) != 18){
					printf("Error: Mac address error.\n");
					break;
				}
				printf("Enter message: ");
				scanf("%s", &buf[strlength(buf) - 1]);

				net_send(buf);
			}else if (strcmp((char *)params[1], "read") == 0){
				int size = net_receive(buf);
				printf("%s", buf);
			}else{
				printf("Net command not found.\n");
			}
			break;
		}
		case COMMAND_HELP: {
			printf("DummyOS Available Commands!\n\n");
			printf("%s\n", man_net);
			printf("%s\n", man_man);
			printf("%s\n", man_time);
			printf("%s\n", man_help);
			printf("%s\n", man_clear);
			printf("%s\n", man_echo);
			printf("%s\n", man_poweroff);
			printf("\n");
			break;
		}
		case COMMAND_MAN: {
			printManPage((char *)params[1]);
			break;
		}
		case COMMAND_POWEROFF: {
			return TRUE;
			break;
		}
		case COMMAND_CLEAR: {
			for (int i = 0; i < SCREEN_SIZE; ++i)
		 		printf("\n");
			break;
		}
		case COMMAND_NULL: {
			// If invalid command, do nothing
			printf("Invalid command.\n");
			break;
		}
	}

	return FALSE;
}

/**
Prints man page for the selected command

@param commandString Pointer to string value of command to run
*/
void printManPage (char *commandString) {
	int command = parseCommand (commandString);

	switch (command) {
		case COMMAND_ECHO: {
			printf("%s\n", man_echo);
			break;
		}
		case COMMAND_TIME: {
			printf("%s\n", man_time);
			break;
		}
		case COMMAND_NET: {
			printf("%s\n", man_net);
			break;
		}
		case COMMAND_HELP: {
			printf("%s\n", man_help);
			break;
		}
		case COMMAND_MAN: {
			printf("%s\n", man_man);
			break;
		}
		case COMMAND_POWEROFF: {
			printf("%s\n", man_poweroff);
			break;
		}
		case COMMAND_CLEAR: {
			printf("%s\n", man_clear);
			break;
		}
		case COMMAND_NULL: {
			if (strlength(commandString) == 0)
				printf("%s\n", man_null);
			else
				printf("%s%s\n", man_invalid, commandString);
			
			break;
		}
	}
		
}