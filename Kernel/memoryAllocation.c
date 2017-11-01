// buddy allocation
#include "memoryAllocation.h"

// struct BuddyAllocator myAllocation;		// TODO: asignado en Kernel
// 										// TODO: pasar esto a que sea un heap

// void * allocPage(int size) {
// 	if(searchForSpace(size, myAllocation) != NULLPTR) {
// 		// reserve space
// 	}
// 	return NULLPTR;
// }

// void * searchForSpace(int size, struct BuddyAllocator myAllocation) {
// 	if(myAllocation.size < size) {		// You will never have enough memory for this process.
// 		return NULLPTR;					// TODO: Should we consider a special msg?
// 	}
// 	return searchForSpaceDFS(size, myAllocation->root);
// }

// void * searchForSpaceDFS(int size, struct BuddyTreeNode treeNode) {
// 	void * newSpace;
// 	if((treeNode.block->size / 2) > size) {
// 		if(isChildFree(treeNode, treeNode->left)) {
// 			newSpace = searchForSpaceDFS(size, treeNode->left);
// 		} else {
// 			if(isChildFree(treeNode, treeNode->right)) {
// 				newSpace = searchForSpaceDFS(size, treeNode->right);
// 			}
// 		}
// 		if(newSpace != NULLPTR) {
// 			treeNode.block->free = SEMIFREE;		// TODO: HOJAS NO PUEDEN ESTAR SEMIFREE
// 		}
// 	} else {
// 		// preguntar ischildfree para saber si los hijos left & right tienen espacio o estan ocupados en su totalidad o parcialmente
// 		if((isChildFree(treeNode, treeNode->left) == 2) && (isChildFree(treeNode, treeNode->right) == 2)) {
// 			treeNode.block->free = OCCUPIED;
// 		}
// 		// hacer un switch en el que si estan libres "reserve" ese lugar
// 	}
// 	return newSpace;
// }

// char isChildFree(struct BuddyTreeNode parent, struct BuddyTreeNode child) {
// 	if(child == NULLPTR) {
// 		if(isNotLeaf(parent.block.size / 2)) {
// 			// "segmentar" memoria para crear un nuevo hijo treeNode
// 			// marcar los nietos con NULLPTR
// 			// linkearle los hijos al padre
// 			// marcar free = FREE
// 		} else {
// 			// marcar free = OCCUPIED
// 		}
// 	}
// 	return child.free;
// }

// char isNotLeaf(int size) {
// 	if(size < MINSIZE) {
// 		return 0;
// 	}
// 	return 1;
// }

int freePage(void * page) {
	return 0;
}