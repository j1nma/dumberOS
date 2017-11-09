#ifndef _MEMORYALLOCATION_H_
#define _MEMORYALLOCATION_H_

#define PAGE_SIZE (1024*4)
#define MEMORYSIZE (1024*1024)
#define SMALLESTBLOCKSIZE (1024)
#define TOTALLEVELS (10)
#define TOTALELEMENTS ((SMALLESTBLOCKSIZE*2)-1)

/*
**	The state is:
**	0 if EMPTY
**	1 if ALMOST-FULL (one of the descendants is occupied)
**	2 if FULL
*/
typedef enum state {INVALID=-1, EMPTY, ALMOST_FULL, FULL} State;

// struct node {
// 	void * base;
// 	int level;
// 	int size;
// 	State state;
// };

void setUpHeapOrganizer(void * memoryBaseFromKernel);
// void fillInformation(struct node * nodeToFill, int level, int elementNumber, int elementsInLevel, int nodeNumber);
int getLevel(int size);
void * allocPage (int size);
void * calculateOffsetFromIndex(int i);
void updateState(int i);
void * findSpaceDFS(int level, int i, int currentLevel);
State findPageDFS(void * page, int i);
void freePage(void * page);

#endif