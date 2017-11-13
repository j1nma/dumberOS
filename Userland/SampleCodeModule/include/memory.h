#include <stdio.h>

#define SMALLESTBLOCKSIZE (1024)
#define PAGE_SIZE (4*1024)

typedef struct header {
	unsigned availableSize;
	unsigned unmutableSize;
	struct header * next;
} Header;

// void * malloc(unsigned nBytes); 
// Header * morecore(unsigned nBytes);
// void free(void *ap);
// --------- For TESTING -----------
void * mymalloc(unsigned nBytes);
Header * morecore(unsigned nBytes);
Header * myfree(void *toFree);