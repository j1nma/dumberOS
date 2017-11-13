#ifndef _MEMORYALLOCATION_H_
#define _MEMORYALLOCATION_H_

#include <math.h>

#define PAGE_SIZE (1024*4)
#define MEMORYSIZE (1024*1024)
#define SMALLESTBLOCKSIZE (1024)
// #define TOTALLEVELS (log2(SMALLESTBLOCKSIZE))
#define TOTALLEVELS (10)
#define TOTALELEMENTS ((SMALLESTBLOCKSIZE*2)-1)

/*
**	The state is:
**	0 if EMPTY
**	1 if ALMOST-FULL (one of the descendants is occupied)
**	2 if FULL
*/
typedef enum state {INVALID=-1, EMPTY, ALMOST_FULL, FULL, FULLANDOCCUPIED} State;

typedef struct header {
	unsigned availableSize;
	unsigned unmutableSize;
	struct header * next;
} Header;

void * memAlloc(unsigned nBytes); 
Header * morecore(unsigned nBytes);
void memFree(void *ap);

void setUpHeapOrganizer(void * memoryBaseFromKernel);
int getLevel(int size);
void * allocSpace(int size);
void * allocNPages(int n);
int leftChildValid(int i);
int rightChildValid(int i);
int getParent(int index);
int getLevelFromIndex(int index);
int getNumberOfElementsInLevel(int level);
int getNodeNumber(int index);
int calculateOffsetFromIndex(int index);
void updateState(int i);
void * findSpaceDFS(int level, int i, int currentLevel);
State findPageDFS(void * page, int i);
void freeSpace(void * page);
void printBuddy();

#endif