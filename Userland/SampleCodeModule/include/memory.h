#include <stdio.h>

typedef struct header {
	unsigned availableSize;
	unsigned unmutableSize;
	struct header * next;
} Header;

void * memAlloc(unsigned nBytes); 
Header * morecore(unsigned nBytes);
void memFree(void *ap);