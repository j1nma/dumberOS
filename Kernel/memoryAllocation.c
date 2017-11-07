
#include "include/memoryAllocation.h"
#include <stdlib.h>
#include <stdio.h>
// #include <math.h>
// #include <stddef.h>	// for NULL to be valid

struct node ** memoryBase;
int totalLevels;
int totalElements;

/* TODO: arreglar que por alguna razon el memoryBase value se cambia. */

int pow(int x, int y) {
	int i;
	int ans = x;
	for(i = 0; i < y-1; i++) {
		ans *= x;
	}
	return ans;
}

void setUpHeapOrganizer() {
	memoryBase = malloc(sizeof(struct node*) * MEMORYPIECES);

	int elementNumber = 0;
	int level = 0;
	int elementsInLevel;
	int j;
	int nodeNumber;

	while(elementNumber < MEMORYPIECES) {

		elementsInLevel = pow(2, level);
		nodeNumber = 0;

		for(j = elementsInLevel; j > 0; j--) {
			struct node* actualNode = (struct node*) malloc(sizeof(struct node)); 
			//printf("%d\n", nodeNumber);
			fillInformation(actualNode, level, elementNumber, elementsInLevel, nodeNumber++); 
			//printf("%p\n", memoryBase); 
			//printf("level: %d elementNumber: %d elementsInLevel: %d nodeNumber: %d \n actualNode->level %d actualNode->state %d actualNode->base %p\n-----------------------------\n",level, elementNumber, elementsInLevel, nodeNumber, actualNode->level, actualNode->state, &actualNode->base);

			memoryBase[elementNumber++] = actualNode;
		}
		level++;
	}
	totalLevels = level;
	totalElements = elementNumber;
}

void fillInformation(struct node * nodeToFill, int level, int elementNumber, int elementsInLevel, int nodeNumber) {
	nodeToFill->level = level;
	nodeToFill->state = EMPTY;
	nodeToFill->size = 0;
	nodeToFill->base = memoryBase + (nodeNumber*(MEMORYSIZE/elementsInLevel));
}


int getLevel(int size){  // por ejemplo si ingreso 75 puedo probar a partir de 2^7 y por lo tanto eso es a partir del 3 nivel [nivel 0 = 1024, nivel 1 = 512, nivel 3 = 128]
	int level = 1;

	while(size > 0) {
		size -= pow(2,level);
		level++;
	}

	return totalLevels - level;
}

void * allocPage(int size) { // indice < level es que es mas grande 
	if(size == 0 || size > PAGE_SIZE) {
		return NULL;
	}

	int level = getLevel(size);

	if(level > totalLevels) {
		return NULL;
	}

	void * goHere = findSpaceDFS(level, 0, 1, size);

	return goHere;
}


// // voy a tener en cuenta que el buddyAllocation no es mas que un binary heap 
// // funcion recursiva
// // paso el level y no el size porque total si el level que queiro llenar esta fully ocupado tengo que recursivamente ocupar tanto como ese nivel ocupe
// // https://www.youtube.com/watch?v=WCm3TqScBM8
// // izquierdo - dato - derecho x heap 

// void * findSpace(int level, int i, int currentLevel){

// 	if(memoryBase[i]->state == FULL){
// 		return NULL; 
// 	} 


// 	if(currentLevel == level){
// 		if(memoryBase[i]->state == ALMOST_FULL){
// 			return NULL; 
// 		}
// 		memoryBase[i]->state = FULL; 
// 		return memoryBase[i]->base; 
// 	}

// 	void * left = findSpace(level,(2*i)+1, currentLevel+1);

// 	//si por la izquierda no puedo, chequeo por la derecha
// 	if(left == NULL){

// 		void * right = findSpace(level, (2*i)+2, currentLevel+1); 

// 		if(right == NULL){
// 			return NULL; 
// 		}

// 		return right; 
// 	}

// 	memoryBase[i]->state = ALMOST_FULL; //me da la sensacion que me va a marcar el 0 como almost_full y no estoy segura si lo tiene que marcar o no como almost_full (????)
// 	return left; 

// }
// voy a tener en cuenta que el buddyAllocation no es mas que un binary heap 
// funcion recursiva
// paso el level y no el size porque total si el level que quiero llenar esta fully ocupado tengo que recursivamente ocupar tanto como ese nivel ocupe
// https://www.youtube.com/watch?v=WCm3TqScBM8
// izquierdo - dato - derecho x heap 

void * findSpaceDFS(int level, int i, int currentLevel, int size) {
	if(memoryBase[i]->state == FULL) {
		return NULL;
	}

	if(currentLevel == level) {
		if(memoryBase[i]->state == ALMOST_FULL) {
			return NULL;
		}
		memoryBase[i]->state = FULL;

		return memoryBase[i]->base;
	}

	void * left = findSpaceDFS(level,(2*i)+1, currentLevel+1, size);

	//si por la izquierda no puedo, chequeo por la derecha
	if(left == NULL) {
		void * right = findSpaceDFS(level, (2*i)+2, currentLevel+1, size);

		if(right == NULL) {
			return NULL;
		}
		return right; 
	}

	memoryBase[i]->state = ALMOST_FULL;	// me da la sensacion que me va a marcar el 0 como almost_full y no estoy segura si lo tiene que marcar o no como almost_full (????)
	return left;	// no tendria que devolver el left->base?
}

int findPageDFS(void * page, int i) {
	if(memoryBase[i]->base == page) {
		if(memoryBase[i]->size) {
			memoryBase[i]->size = 0;
			memoryBase[i]->state = EMPTY;
			return memoryBase[i]->state;
		} else {
			if(2*i+1 < totalElements) {
				int state = findPageDFS(page, 2*i+1);
				if(state == EMPTY) {					// it has to be your left child or one of its descendants
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
						//	page not valid
					}
				} else if(state == ALMOST_FULL) {
					memoryBase[i]->state = ALMOST_FULL;
				} else {
					return INVALID;	// page was already freed, nothing to do here
				}
				return memoryBase[i]->state;
			}
			return INVALID;			// page out of range, nothing to do here
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
