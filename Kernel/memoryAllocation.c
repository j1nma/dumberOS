#include "include/memoryAllocation.h"
#include <stdlib.h>
#include <stdio.h>
// #include <math.h>
// #include <stddef.h>	// Might be required in Linux for NULL to be valid.

void * memoryBase;// = buddyAllocationMemory;	// in Kernel.c
char memManager[TOTALELEMENTS] = {EMPTY};
// int totalLevels;
// int totalElements;

// int pow(int x, int y) {
// 	int i;
// 	int ans = x;
// 	for(i = 0; i < y-1; i++) {
// 		ans *= x;
// 	}
// 	return ans;
// }

void setUpHeapOrganizer(void * memoryBaseFromKernel) {
	int i;
	for(i = 0; i < TOTALELEMENTS; i++) {
		memManager[i] = EMPTY;
	}
	// FOR TESTING:
	// memoryBase = malloc(sizeof(1 mega?));
	memoryBase = memoryBaseFromKernel;
}
// 	memoryBase = malloc(sizeof(struct node*) * MEMORYPIECES);

// 	int elementNumber = 0;
// 	int level = 0;
// 	int elementsInLevel;
// 	int j;
// 	int nodeNumber;

// 	while(elementNumber < MEMORYPIECES) {

// 		elementsInLevel = pow(2, level);
// 		nodeNumber = 0;

// 		for(j = elementsInLevel; j > 0; j--) {
// 			struct node* actualNode = (struct node*) malloc(sizeof(struct node)); 
// 			fillInformation(actualNode, level, elementNumber, elementsInLevel, nodeNumber++); 
// 			//printf("level: %d elementNumber: %d elementsInLevel: %d nodeNumber: %d \n actualNode->level %d actualNode->state %d actualNode->base %p\n-----------------------------\n",level, elementNumber, elementsInLevel, nodeNumber, actualNode->level, actualNode->state, actualNode->base);
// 			memoryBase[elementNumber++] = actualNode;
// 		}
// 		level++;
// 	}
// 	totalLevels = level;
// 	totalElements = elementNumber;
// }

// void fillInformation(struct node * nodeToFill, int level, int elementNumber, int elementsInLevel, int nodeNumber) {
// 	nodeToFill->level = level;
// 	nodeToFill->state = EMPTY;
// 	nodeToFill->size = 0;
// 	nodeToFill->base = memoryBase + (nodeNumber*(MEMORYSIZE/elementsInLevel));
// }


// por ejemplo si ingreso 75 puedo probar a partir de 2^7 y por lo tanto eso es a partir del 3 nivel [nivel 0 = 1024, nivel 1 = 512, nivel 3 = 128]

/*
** Max level is 10 (2^10 equals 1024 and it's the small assignable block size).
*/
int getLevel(int size){
// 	int level = 1;

// 	while(size > 0) {
// 		size -= pow(2,level);
// 		level++;
// 	}
// 	return totalLevels - level;
	int level = 0;

	while(size > SMALLESTBLOCKSIZE) {
		size /= 2;
		level++;
	}

	return TOTALLEVELS - level;
}

/*
** Minimum allocation block size is 1024
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

int calculateOffsetFromIndex(int i) {
	// TODO
	return 0;
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
	} else if(memManager[2*i+1] == FULL && memManager[2*i+2] == FULL) {
		memManager[i] = FULL;
	} else {
		memManager[i] = ALMOST_FULL;
	}
}

/*
** Buddy Allocation is nothing more than a binary heap.
**
** Recursive Function
** Paso el level y no el size porque total si el level que quiero llenar esta fully ocupado
** tengo que recursivamente ocupar tanto como ese nivel ocupe.
** https://www.youtube.com/watch?v=WCm3TqScBM8
** izquierdo - dato - derecho x heap
*/
void * findSpaceDFS(int level, int i, int currentLevel) {
	void * ans;
	void * left;
	void * right;

	if(memManager[i] == FULL) {
		return NULL;
	}

	if(currentLevel == level) {
		if(memManager[i] == ALMOST_FULL) {
			return NULL;
		} else {
			memManager[i] = FULL;
			return memoryBase + (calculateOffsetFromIndex(i));			// TODO: el calculo del salto falta aca
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

	// if(memoryBase[i]->state == FULL) {
	// 	return NULL;
	// }

	// if(currentLevel == level) {
	// 	if(memoryBase[i]->state == ALMOST_FULL) {
	// 		return NULL;
	// 	}
	// 	memoryBase[i]->state = FULL;

	// 	return memoryBase[i]->base;
	// }

	// void * left = findSpaceDFS(level,(2*i)+1, currentLevel+1, size);

	// //si por la izquierda no puedo, chequeo por la derecha
	// if(left == NULL) {
	// 	void * right = findSpaceDFS(level, (2*i)+2, currentLevel+1, size);

	// 	if(right == NULL) {
	// 		return NULL;
	// 	}
	// 	return right; 
	// }

	// memoryBase[i]->state = ALMOST_FULL;	// me da la sensacion que me va a marcar el 0 como almost_full y no estoy segura si lo tiene que marcar o no como almost_full (????)
	// return left;
}

// int getNewState(int descendantState, int parentState, int child) {
// 	switch(descendantState) {
// 		case EMPTY:									// it has to be your left/right child or one of its descendants
// 			if(parentState == FULL) {				// if it was then you must update your state
// 				return ALMOST_FULL;
// 			} else {								// might depend on your other child
// 				if(child < totalElements) {
// 					if(memoryBase[child]->state) {
// 						return ALMOST_FULL;
// 					} else {
// 						return EMPTY;
// 					}
// 				}
// 				//	page not valid
// 			}
// 			break;
// 		case ALMOST_FULL:
// 			return ALMOST_FULL;
// 		default:
// 			return INVALID;
// 	}
// }

int leftChildValid(int i) {
	return ((2*i) + 1) < TOTALELEMENTS;
}

int rightChildValid(int i) {
	return ((2*i) + 2) < TOTALELEMENTS;
}

/*
** 
*/
State findPageDFS(void * page, int i) {
	// void * aux = memoryBase + calculateOffsetFromIndex(i);
	// if(aux == page) {
	if(memoryBase + calculateOffsetFromIndex(i) == page) {
		if(memManager[i] == FULL) {
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

// int main(int argc, char const *argv[])
// {
// 	startHeap(); 
// 	//printf("totalLevels %d\n", totalLevels);
// 	//printf("getLevel(75) %d \n", getLevel(75));
// 	return 0;
// }
// int main(int argc, char const *argv[])			// TODO: should move call to setUpHeapOrganizer to Kernel.c when implemented in OS
// {
// 	setUpHeapOrganizer(malloc(sizeof(1024*1024))); 
// 	printf("totalLevels %d\n", TOTALLEVELS);
// 	printf("getLevel(1500) %d \n", getLevel(1500));
// 	void * test = allocSpace(1500);
// 	printf("%p\n", test);
// 	printf("%d\n", findPageDFS(test+2, 0));
// 	printf("%d\n", findPageDFS(test, 0));
// 	printf("%s\n", "Todo bien.");
// 	return 0;
// }
