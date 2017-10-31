#ifndef _MEMORYALLOCATION_H_
#define _MEMORYALLOCATION_H_

/*
**	The char free is:
**	0 if occupied
**	1 if semi-free (one of the descendants is occupied)
**	2 if free
*/
enum memState {occupied = 0, semi-free, free};

struct memBlock {
	char free;
	int size;
};

struct BuddyTreeNode {
	struct memBlock block;
	struct memBlock * left;
	struct memBlock * right;
};

struct BuddyAllocator {
	struct buddyTreeNode * root;
	int size;
} buddyAllocator;

void * allocPage(int size);
void * searchForSpace(int size, BuddyAllocator myAllocation);
void * searchForSpaceDFS(int size, BuddyTreeNode treeNode);
char isChildFree(BuddyTreeNode treeNode);
int freePage(void * page);

#endif