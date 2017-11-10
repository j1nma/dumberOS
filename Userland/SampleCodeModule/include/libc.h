#include <stdint.h>
#include <stdarg.h>

typedef int (*EntryPoint)();

#define TRUE 1
#define FALSE 0
#define MAX_PARAMS_SHELL 5

void printf(char *format, ...);
void scanf(char *format, ...);
char** str_split(char *str);
int strcmp(char *a, char *b);
char* strconcat(char *a, char *b);
int strlength(char *c);
void net_send(char *s);
int net_receive(char *s);
void *malloc(int size);
void *calloc(int size);
void free(void *ptr);
void getTime(char *ptr);
void haltAsm();
void halt();
int getPid();
void LoopNop();
int killProcess(int pid);

void send(char *, int pid);
char * receive();

int createProcess(EntryPoint entryPoint);

/* Mutual exclusion */
int * newBolt();
int acquireBolt(int * bolt);
int releaseBolt(int * bolt);
int deleteBolt(int * ptr);
void * thread_do(void * arg);