#include "memoryAllocation.h"

void * memoryBase;// = buddyAllocationMemory;	// in Kernel.c
char memManager[TOTALELEMENTS] = {EMPTY};

void setUpHeapOrganizer(void * memoryBaseFromKernel) {
	int i;
	for(i = 0; i < TOTALELEMENTS; i++) {
		memManager[i] = EMPTY;
	}
	memoryBase = memoryBaseFromKernel;
}

/*
** Max level is 10 (2^10 equals 1024 and it's the small assignable block size).
*/
int getLevel(int size){
	int level = 0;

	while(size > SMALLESTBLOCKSIZE) {
		size /= 2;
		level++;
	}
	return MAXLEVELS - level;
}

/*
** Minimum allocation block size is 1024.
*/
void * allocSpace(int size) {
	int level;
	void * ans;
	if(size == 0) {
		return MYNULL;
	}
	level = getLevel(size);		// indice < level es que es mas grande 
								// level ahora tiene el 2^level que necesita
	if(level > MAXLEVELS) {
		return MYNULL;
	}
	if(level < 0) {
		return MYNULL;
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

int getLeftChild(int index) {
	return 2 * index + 1;
}

int getRightChild(int index) {
	return 2 * index + 2;
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
	// Instead of using pow we do it with a shift so as to not include math.h or other libraries.
	return 1 << level;
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
void updateState(int index) {
	if(memManager[getLeftChild(index)] == ALMOST_FULL || memManager[getRightChild(index)] == ALMOST_FULL) {
		memManager[index] = ALMOST_FULL;
	} else if(memManager[getLeftChild(index)] == EMPTY && memManager[getRightChild(index)] == EMPTY) {
		memManager[index] = EMPTY;
	} else if((memManager[getLeftChild(index)] == FULL || memManager[getLeftChild(index)] == FULLANDOCCUPIED) && (memManager[getRightChild(index)] == FULL || memManager[getRightChild(index)] == FULLANDOCCUPIED)) {
		memManager[index] = FULL;
	} else {
		memManager[index] = ALMOST_FULL;
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
void * findSpaceDFS(int level, int index, int currentLevel) {
	void * ans;
	// Fail-safe checks
	// if(memManager[i] == FULL) {
	if(!isIndexValid(index) || memManager[index] == FULLANDOCCUPIED) {
		return MYNULL;
	}

	// Base case
	if(currentLevel == level) {
		if(memManager[index] == ALMOST_FULL) {
			return MYNULL;
		} else if(memManager[index] == FULL) {
			return MYNULL;
		} else {
			memManager[index] = FULLANDOCCUPIED;
			return memoryBase + (calculateOffsetFromIndex(index));
		}
	}

	// Recursive case
	ans = findSpaceDFS(level, getLeftChild(index), currentLevel + 1);
	if(ans == MYNULL) {	// If left gives back NULL I try through the right side.
		ans = findSpaceDFS(level, getRightChild(index), currentLevel + 1);
		if(ans == MYNULL) {
			return MYNULL;
		}
	}
	updateState(index);	// I update my state accordingly.
	return ans;
}

int isIndexValid(int index) {
	return index < TOTALELEMENTS;
}

/*
** Finds the page and marks it as free in the heap.
*/
State findPageDFS(void * page, int index) {
	// Fail-safe checks.
	if(!isIndexValid(index)) {
		return INVALID;
	}

	// Base case.
	if(memoryBase + calculateOffsetFromIndex(index) == page) {
		if(memManager[index] == FULLANDOCCUPIED) {					// Found the base pointer I was looking for!
			memManager[index] = EMPTY;								// Found the actual page I was looking for!
			return memManager[index];
		} else {
			int state = findPageDFS(page, getLeftChild(index));
			if(state == INVALID) {
				return INVALID;										// Freeing a page that's not reserved!
			}
			updateState(index);
			return memManager[index];
		}
	} else {
		// Recursive case.

		// Keep searching for page.
		// It can be a descendant of the left child or a descendant of the right child.
		// Starting with left child.
		int stateLeft = findPageDFS(page, getLeftChild(index));
		if(stateLeft != INVALID) {
			updateState(index);
			return memManager[index];
		} else {
			int stateRight = findPageDFS(page, getRightChild(index));
			if(stateRight != INVALID) {
				updateState(index);
				return memManager[index];
			} else {
				return INVALID;								// Not found on this branch.
			}
		}
		return INVALID;
	}
}

void freeSpace(void * page) {
	findPageDFS(page, 0);
}