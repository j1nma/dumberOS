#include "include/memoryAllocation.h"
#include <stdlib.h>
#include <stdio.h>
// #include <stddef.h>	// Might be required in Linux for NULL to be valid.

void * memoryBase;// = buddyAllocationMemory;	// in Kernel.c
char memManager[TOTALELEMENTS] = {EMPTY};

void setUpHeapOrganizer(void * memoryBaseFromKernel) {
	int i;
	for(i = 0; i < TOTALELEMENTS; i++) {
		memManager[i] = EMPTY;
	}
	memoryBase = memoryBaseFromKernel;
}
// 	nodeToFill->base = memoryBase + (nodeNumber*(MEMORYSIZE/elementsInLevel));

/*
** Max level is 10 (2^10 equals 1024 and it's the small assignable block size).
*/
int getLevel(int size){
	int level = 0;

	while(size > SMALLESTBLOCKSIZE) {
		size /= 2;
		level++;
	}
	return TOTALLEVELS - level;
}

/*
** Minimum allocation block size is 1024.
*/
void * allocSpace(int size) {
	int level;
	void * ans;
	if(size == 0) {
		return NULL;
	}
	level = getLevel(size);		// indice < level es que es mas grande 
								// level ahora tiene el 2^level que necesita
	if(level > TOTALLEVELS) {
		return NULL;
	}
	ans = findSpaceDFS(level, 0, 0);

	return ans;
}

void * allocNPages(int n) {
	return allocSpace(n * PAGE_SIZE);
}

int getParent(int index) {
	return (index-1)/2;
}

int getLevelFromIndex(int index) {
	int level = 0;
	while(index != 0) {
		index = getParent(index);
		level++;
	}
	return level;
}

/*
** Calculates the number of elements in the given level.
** The Buddy Allocator will have 2^n elements in
** level n (the first level is 2^0 = 1 element which
** represents the whole memory that the Buddy Allocator has).
*/
int getNumberOfElementsInLevel(int level) {
	// Instead of using pow we do it with a for cycle so as to not include math.h or other libraries.
	int base = 2;
	int i;
	int numberOfElementsInLevel = 1;

	for (i = 0 ; i<level; i++) {
		numberOfElementsInLevel *= base;
	}
	return numberOfElementsInLevel;
}

int getNodeNumber(int index) {
	int nodeNumber = 0;
	int level = getLevelFromIndex(index);
	int elementsInLevel = getNumberOfElementsInLevel(level);
	int startingPointOfLevel = elementsInLevel - 1;

	while(index != startingPointOfLevel) {
		nodeNumber++;
		index--;
	}
	return nodeNumber;
}

int getSizeofBlockFromIndex(int index) {
	return MEMORYSIZE/getNumberOfElementsInLevel(getLevelFromIndex(index));
}

int calculateOffsetFromIndex(int index) {
	return getNodeNumber(index) * getSizeofBlockFromIndex(index);
}

/*
** Updates the state of the memory manager at index i depending on the state of its children.
** Prioratizes asking if a child is ALMOST_FULL because it's the most common state.
*/
void updateState(int i) {
	if(memManager[2*i+1] == ALMOST_FULL || memManager[2*i+2] == ALMOST_FULL) {
		memManager[i] = ALMOST_FULL;
	} else if(memManager[2*i+1] == EMPTY && memManager[2*i+2] == EMPTY) {
		memManager[i] = EMPTY;
	} else if((memManager[2*i+1] == FULL || memManager[2*i+1] == FULLANDOCCUPIED) && (memManager[2*i+2] == FULL || memManager[2*i+2] == FULLANDOCCUPIED)) {
		memManager[i] = FULL;
	} else {
		memManager[i] = ALMOST_FULL;
	}
}

/*
** Buddy Allocation is nothing more than a binary heap.
**
** Recursive Function
** Paso el level y no el size porque total si el level que quiero llenar esta
** fully ocupado tengo que recursivamente ocupar tanto como ese nivel ocupe.
** https://www.youtube.com/watch?v=WCm3TqScBM8
** izquierdo - dato - derecho x heap
*/
void * findSpaceDFS(int level, int i, int currentLevel) {
	void * ans;
	void * left;
	void * right;

	// if(memManager[i] == FULL) {
	if(memManager[i] == FULLANDOCCUPIED) {
		return NULL;
	}

	if(currentLevel == level) {
		if(memManager[i] == ALMOST_FULL) {
			return NULL;
		} else if(memManager[i] == FULL) {
			return NULL;
		} else {
			memManager[i] = FULLANDOCCUPIED;
			return memoryBase + (calculateOffsetFromIndex(i));
		}
	}

	if(!leftChildValid(i)) {
		return NULL;
	}

	left = findSpaceDFS(level, ((2*i)+1), currentLevel + 1);

	// If left gives back NULL I try through the right side.
	if(left == NULL) {
		if(!rightChildValid(i)) {
			return NULL;
		}
		right = findSpaceDFS(level, (2*i)+2, currentLevel + 1);
		if(right == NULL) {
			return NULL;
		}
		ans = right;
	} else {
		ans = left;
	}
	// I update my state accordingly.
	updateState(i);

	return ans;
}

int leftChildValid(int i) {
	return ((2*i) + 1) < TOTALELEMENTS;
}

int rightChildValid(int i) {
	return ((2*i) + 2) < TOTALELEMENTS;
}

/*
** Finds the page and marks it as free in the heap.
*/
State findPageDFS(void * page, int i) {
	// void * aux = memoryBase + calculateOffsetFromIndex(i);
	// if(aux == page) {
	if(memoryBase + calculateOffsetFromIndex(i) == page) {
		if(memManager[i] == FULLANDOCCUPIED) {
			memManager[i] = EMPTY;								// Found the page I was looking for!
			return memManager[i];
		} else {
			if(leftChildValid(i)) {							// It's a left child (they share the same *base).
				int state = findPageDFS(page, 2*i+1);
				if(state == INVALID) {
					return INVALID;								// Freeing a page that's not reserved!
				}
				updateState(i);
				return memManager[i];
			} else {
				return INVALID;									// Freeing a page that's not reserved! (Page out of range)
			}
		}
	} else {
		// Keep searching for page.
		// It can be a descendant of the left child or a descendant of the right child.
		// Starting with left child.
		if(leftChildValid(i)) {
			int stateLeft = findPageDFS(page, 2*i+1);
			if(stateLeft != INVALID) {
				updateState(i);
				return memManager[i];
			} else if(rightChildValid(i)) {
				int stateRight = findPageDFS(page, 2*i+2);
				if(stateRight != INVALID) {
					updateState(i);
					return memManager[i];
				} else {
					return INVALID;								// Not found on this branch.
				}
			}
			return INVALID;
		}
		return INVALID;
	}
}

void freePage(void * page) {
	findPageDFS(page, 0);
}

// void printBuddy() {
// 	int i;
// 	int level = 0, elementsInRow = 1;
// 	for(i = 0; i < TOTALELEMENTS; i++) {
// 		printf("%d ", memManager[i]);
// 		if(pow(2, level) == elementsInRow) {
// 			printf("|");
// 			level++;
// 			elementsInRow = 1;
// 		} else {
// 			elementsInRow++;
// 		}
// 	}
// 	printf("\n");
// }

// int main(int argc, char const *argv[])
// {
// 	void * mymemory = malloc(sizeof(1024*1024));
// 	setUpHeapOrganizer(mymemory);
// 	printf("totalLevels %d\n", (int) TOTALLEVELS);
// 	printf("getLevel(1500) %d \n", getLevel(1500));
// 	void * test1 = allocSpace(1500);
// 	// void * test2 = allocSpace(800);
// 	void * test3 = allocSpace(1500);
// 	printf("Test1 base: %p\n", test1);
// 	// printf("Test2 base: %p\n", test2);
// 	printf("Test3 base: %p\n", test3);
// 	printBuddy();
// 	printf("Freeing fake page: %d\n", findPageDFS(test1+2, 0));
// 	printBuddy();
// 	printf("Freeing test1: %d\n", findPageDFS(test1, 0));
// 	printBuddy();
// 	// printf("Freeing test2: %d\n", findPageDFS(test2, 0));
// 	printf("Freeing test3: %d\n", findPageDFS(test3, 0));
// 	printBuddy();
// 	free(mymemory);
// 	// printf("%d\n", sizeof(char));
// 	return 0;
// }
