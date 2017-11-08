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
void * allocPage(int size) {
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
			//
			memManager[i] = FULL;
			return memoryBase + (0);			// TODO: el calculo del salto falta aca
		}
	}

	left = findSpaceDFS(level, ((2*i)+1), currentLevel + 1);

	// If left gives back NULL I try through the right side.
	if(left == NULL) {
		right = findSpaceDFS(level, (2*i)+2, currentLevel + 1);
		if(right == NULL) {
			return NULL;
		}
		ans = right;
	} else {
		ans = left;
	}

	// I update my state accordingly.
	if(memManager[2*i+1] == FULL && memManager[2*i+2] == FULL) {
		memManager[i] = FULL;
	} else {
		memManager[i] = ALMOST_FULL;
	}
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

int getNewState(int descendantState, int parentState, int child) {
	switch(descendantState) {
		case EMPTY:									// it has to be your left/right child or one of its descendants
			if(parentState == FULL) {				// if it was then you must update your state
				return ALMOST_FULL;
			} else {								// might depend on your other child
				if(child < totalElements) {
					if(memoryBase[child]->state) {
						return ALMOST_FULL;
					} else {
						return EMPTY;
					}
				}
				//	page not valid
			}
			break;
		case ALMOST_FULL:
			return ALMOST_FULL;
		default:
			return INVALID;
	}
}

/*
** 
*/
int findPageDFS(void * page, int i) {
	if(memoryBase[i]->base == page) {
		if(memoryBase[i]->size) {			// If it gets here, it's either this one or a left child.
			memoryBase[i]->size = 0;		// If it gets here, I've found it!
			memoryBase[i]->state = EMPTY;
			return memoryBase[i]->state;	// Return new state so "parents" can update themselves as needed.
		} else {
			if(2*i+1 < totalElements) {
				int state = findPageDFS(page, 2*i+1);	// Else look for the left child.
				memoryBase[i]->state = getNewState(state, memoryBase[i]->state, 2*i+2);
				return memoryBase[i]->state;
			}
			return INVALID;					// Looking in the wrong branch (page out of range), nothing to do here.
		}
	} else {
		if(2*i+2 < totalElements) {
			int stateRight = findPageDFS(page, 2*i+2);
			if(stateRight != INVALID) {
				if(stateRight == EMPTY) {
					if(memoryBase[2*i+1]->state) {
						memoryBase[i]->state = ALMOST_FULL;
					} else {
						memoryBase[i]->state = EMPTY;
					}
				} else {
					memoryBase[i]->state = ALMOST_FULL;
				}
			} else {
				int stateLeft = findPageDFS(page, 2*i+1);
				if(stateLeft == INVALID) {
					return INVALID;
				} else if(stateLeft == EMPTY) {
					if(memoryBase[i]->state == FULL) {		// if it was then you must update your state
						memoryBase[i]->state = ALMOST_FULL;
					} else {								// might depend on your right child
						if(2*i+2 < totalElements) {
							if(memoryBase[2*i+2]->state) {
								memoryBase[i]->state = ALMOST_FULL;
							} else {
								memoryBase[i]->state = EMPTY;
							}
						}
					}
				} else {
					memoryBase[i]->state = ALMOST_FULL;
				}
			}
			return memoryBase[i]->state;
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
// 	setUpHeapOrganizer(); 
// 	//printf("totalLevels %d\n", totalLevels);
// 	//printf("getLevel(75) %d \n", getLevel(75));
// 	return 0;
// }
