#ifndef _MEMORYALLOCATION_H_
#define _MEMORYALLOCATION_H_

#define NULLPTR (void*) -1

/*
**	The char free is:
**	0 if occupied
**	1 if semi-free (one of the descendants is occupied)
**	2 if free
*/
enum memState {OCCUPIED = 0, SEMIFREE, FREE};

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
};

void * allocPage(int size);
void * searchForSpace(int size, struct BuddyAllocator myAllocation);
void * searchForSpaceDFS(int size, struct BuddyTreeNode treeNode);
char isChildFree(struct BuddyTreeNode treeNode);
char isNotLeaf(int size);
int freePage(void * page);

#endif