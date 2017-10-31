#ifndef _MEMORYALLOCATION_H_
#define _MEMORYALLOCATION_H_

struct memBlock {
	char free;		// is there anything smaller?
	int size;
};

struct buddyTreeNode {
	struct memBlock block;
	struct memBlock * left;
	struct memBlock * right;
};

struct buddyAllocator {
	struct buddyTree * root;
	int size;
} buddyAllocator;

void * allocPage(int size);

int freePage(void * page);

#endif