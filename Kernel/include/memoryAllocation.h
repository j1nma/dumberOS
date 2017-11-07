#ifndef _MEMORYALLOCATION_H_
#define _MEMORYALLOCATION_H_

// void * allocPage(int size);
// void * searchForSpace(int size, struct BuddyAllocator myAllocation);
// void * searchForSpaceDFS(int size, struct BuddyTreeNode treeNode);
// char isChildFree(struct BuddyTreeNode treeNode);
// char isNotLeaf(int size);

#define MEMORYSIZE 1024
#define MEMORYPIECES MEMORYSIZE/2
#define PAGE_SIZE 1024*4
#define INVALID -1

/*
**	The state is:
**	0 if EMPTY
**	1 if ALMOST-FULL (one of the descendants is occupied)
**	2 if FULL
*/
typedef enum state {EMPTY=0, ALMOST_FULL, FULL} State;

struct node {
	void * base;
	int level;
	int size;
	State state;
};

void setUpHeapOrganizer();
void fillInformation(struct node * nodeToFill, int level, int elementNumber, int elementsInLevel, int nodeNumber);
int getLevel(int size);
void * allocPage (int size);
void * findSpaceDFS(int level, int i, int currentLevel, int size);
int findPageDFS(void * page, int i);
void freePage(void * page);

#endif