#include "include/memoryAllocation.h"
// #include <stdlib.h>
// #include <stdio.h>

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
	if(level < 0) {
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
		return NULL;
	}

	// Base case
	if(currentLevel == level) {
		if(memManager[index] == ALMOST_FULL) {
			return NULL;
		} else if(memManager[index] == FULL) {
			return NULL;
		} else {
			memManager[index] = FULLANDOCCUPIED;
			return memoryBase + (calculateOffsetFromIndex(index));
		}
	}

	// Recursive case
	ans = findSpaceDFS(level, getLeftChild(index), currentLevel + 1);
	if(ans == NULL) {	// If left gives back NULL I try through the right side.
		ans = findSpaceDFS(level, getRightChild(index), currentLevel + 1);
		if(ans == NULL) {
			return NULL;
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

// Para testear hay que descomentar las libraries (arriba de todo) e incluir la math library con -lm

// 	void * mymemory = malloc(sizeof(1024*1024));
// 	setUpHeapOrganizer(mymemory);
// 	printf("totalLevels %d\n", (int) TOTALLEVELS + 1);
// 	printf("getLevel(1500) %d \n", getLevel(1500));
// 	void * test1 = allocSpace(1500);
// 	void * test3 = allocSpace(1500);
// 	void * test2 = allocSpace(800);
// 	void * test4 = allocSpace(1024*1024);
// 	void * test5 = allocSpace(1500);
// 	printf("Test1 base: %p\n", test1);
// 	printf("Test2 base: %p\n", test2);
// 	printf("Test3 base: %p\n", test3);
// 	printf("Test4 base: %p\n", test4);
// 	printf("Test5 base: %p\n", test5);
// 	void * test6 = allocSpace(1500);
// 	void * test7 = allocSpace(1500);
// 	void * test8 = allocSpace(1500);
// 	void * test9 = allocSpace(1500);
// 	void * test10 = allocSpace(131071);
// 	void * test11 = allocSpace(131072);
// 	void * test12 = allocSpace(131073);
// 	void * test13 = allocSpace(131071);
// 	void * test14 = allocSpace(131071);
// 	void * test15 = allocSpace(131071);
// 	void * test16 = allocSpace(1500);
// 	void * test17 = allocSpace(1500);
// 	void * test18 = allocSpace(1500);
// 	void * test19 = allocSpace(1500);
// 	void * test20 = allocSpace(1500);
// 	void * test21 = allocSpace(1500);
// 	void * test22 = allocSpace(1500);
// 	void * test23 = allocSpace(1500);
// 	void * test24 = allocSpace(1500);
// 	void * test25 = allocSpace(131071);
// 	printf("Test10 base: %p\n", test10);
// 	printf("Test11 base: %p\n", test11);
// 	printf("Test12 base: %p\n", test12);
// 	printf("Test13 base: %p\n", test13);
// 	printf("Test14 base: %p\n", test14);
// 	printf("Test15 base: %p\n", test15);
// 	printf("Test16 base: %p\n", test16);
// 	printf("Test25 base: %p\n", test25);
// 	printBuddy();
// 	printf("Freeing fake page: %d\n", findPageDFS(test1+2, 0));
// 	printBuddy();
// 	printf("Freeing test1: %d\n", findPageDFS(test1, 0));
// 	printBuddy();
// 	printf("Freeing test2: %d\n", findPageDFS(test2, 0));
// 	printf("Freeing test3: %d\n", findPageDFS(test3, 0));
// 	printBuddy();
// 	printf("Freeing test4 (should not work): %d\n", findPageDFS(test4, 0));
// 	printf("Freeing test5: %d\n", findPageDFS(test5, 0));
// 	printf("Freeing test6: %d\n", findPageDFS(test6, 0));
// 	printf("Freeing test7: %d\n", findPageDFS(test7, 0));
// 	printf("Freeing test8: %d\n", findPageDFS(test8, 0));
// 	printf("Freeing test9: %d\n", findPageDFS(test9, 0));
// 	printBuddy();
// 	printf("Freeing test10: %d\n", findPageDFS(test10, 0));
// 	printf("Freeing test11: %d\n", findPageDFS(test11, 0));
// 	printf("Freeing test12: %d\n", findPageDFS(test12, 0));
// 	printf("Freeing test13: %d\n", findPageDFS(test13, 0));
// 	printf("Freeing test14: %d\n", findPageDFS(test14, 0));
// 	printf("Freeing test15: %d\n", findPageDFS(test15, 0));
// 	printf("Freeing test16: %d\n", findPageDFS(test16, 0));
// 	printf("Freeing test17: %d\n", findPageDFS(test17, 0));
// 	printf("Freeing test18: %d\n", findPageDFS(test18, 0));
// 	printf("Freeing test19: %d\n", findPageDFS(test19, 0));
// 	printf("Freeing test20: %d\n", findPageDFS(test20, 0));
// 	printf("Freeing test21: %d\n", findPageDFS(test21, 0));
// 	printf("Freeing test22: %d\n", findPageDFS(test22, 0));
// 	printf("Freeing test23: %d\n", findPageDFS(test23, 0));
// 	printf("Freeing test24: %d\n", findPageDFS(test24, 0));
// 	printf("Freeing test25: %d\n", findPageDFS(test25, 0));
// 	printBuddy();

// 	free(mymemory);

// 	// printf("%d\n", sizeof(char));
// 	return 0;
// }
