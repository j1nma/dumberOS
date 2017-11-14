#ifndef _MEMORYALLOCATION_H_
#define _MEMORYALLOCATION_H_


#define PAGE_SIZE (1024*4)
#define MEMORYSIZE (1024*1024)
#define SMALLESTBLOCKSIZE (1024)
// #define TOTALLEVELS (log2(SMALLESTBLOCKSIZE))
#define MAXLEVELS (10)
#define TOTALELEMENTS ((SMALLESTBLOCKSIZE*2)-1)
#define MYNULL (void*)0

/*
**	The state is:
**	0 if EMPTY
**	1 if ALMOST-FULL (one of the descendants is occupied)
**	2 if FULL
*/
typedef enum state {INVALID=-1, EMPTY, ALMOST_FULL, FULL, FULLANDOCCUPIED} State;


void setUpHeapOrganizer(void * memoryBaseFromKernel);
int getLevel(int size);
void * allocSpace(int size);
void * allocNPages(int n);
int isIndexValid(int index);
int getLeftChild(int index);
int getRightChild(int index);
int getParent(int index);
int getLevelFromIndex(int index);
int getNumberOfElementsInLevel(int level);
int getNodeNumber(int index);
int calculateOffsetFromIndex(int index);
void updateState(int index);
void * findSpaceDFS(int level, int index, int currentLevel);
State findPageDFS(void * page, int index);
void freeSpace(void * page);
void printBuddy();

#endif