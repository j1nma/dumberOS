#include <stdint.h>
#include <stdarg.h>

typedef int (*EntryPoint)();

#define TRUE 1
#define FALSE 0
#define MAX_PARAMS_SHELL 5

void initMemory();

void *malloc(int size);

void *calloc(int size);

void free(void *ptr);

void printf(char * format, ...);

int scanf(char * format, ...);

char ** str_split(char * str);

int strcmp(char * a, char * b);

char * strconcat(char * a, char * b);

int strlength(char * c);

void net_send(char * s);

int net_receive(char * s);

void * alloc(int size);

void freeP(void *ptr);

void getTime(char *ptr);

void haltAsm();

void halt();

int getPid();

int getRSP();

void LoopNop();

int killProcess(int pid);

void listProcesses();

int createProcess(EntryPoint entryPoint);

/* Message passing*/
int send(char *, int pid);

char * receive();

/* Mutual exclusion */
int down();

int up();