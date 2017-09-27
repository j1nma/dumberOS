#include <libc.h>

#define COMMAND_NULL -1
#define COMMAND_ECHO 0
#define COMMAND_TIME 1
#define COMMAND_NET 2
#define COMMAND_HELP 3
#define COMMAND_MAN 4
#define COMMAND_POWEROFF 5
#define COMMAND_CLEAR 6

#define SCREEN_SIZE 78

void printManPage (char *command);
int runCommand(int command, char **params);
int parseCommand(char *params);