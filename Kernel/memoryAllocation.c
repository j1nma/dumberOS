// buddy allocation
// include memalloc.h
#include "memoryAllocation.h"

struct BuddyAllocator myAllocation;		// TODO: creado en Kernel?

void * allocPage(int size) {
	if(searchForSpace(size, myAllocation) != NULL) {
		// reserve space
	}
	return NULL;
}

void * searchForSpace(int size, BuddyAllocator myAllocation) {
	if(myAllocation.size < size) {		// You will never have enough memory to do this.
		return NULL;					// TODO: Should we consider a special msg?
	}
	return searchForSpaceDFS(size, myAllocation->root);
}

void * searchForSpaceDFS(int size, BuddyTreeNode treeNode) {
	void * newSpace;
	if((treeNode.block->size / 2) > size) {
		if(isChildFree(treeNode->left)) {
			newSpace = searchForSpaceDFS(size, treeNode->left);
		} else {
			if(isChildFree(treeNode->right)) {
				newSpace = searchForSpaceDFS(size, treeNode->right);
			}
		}
	} else {
		// preguntar ischildfree para saber si los hijos left & right tienen espacio o estan ocupados en su totalidad o parcialmente
		// hacer un switch en el que si estan libres "reserve" ese lugar
	}
	return newSpace;
}

char isChildFree(BuddyTreeNode treeNode) {
	if(treeNode == NULL) {
		// "segment" memory to create new treeNode
	}
	return treeNode.free;
}

int freePage(void * page) {
	return 0;
}