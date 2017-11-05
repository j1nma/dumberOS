// // // buddy allocation
// // #include "memoryAllocation.h"

// // // struct BuddyAllocator myAllocation;		// TODO: asignado en Kernel
// // // 										// TODO: pasar esto a que sea un heap

// // // void * allocPage(int size) {
// // // 	if(searchForSpace(size, myAllocation) != NULLPTR) {
// // // 		// reserve space
// // // 	}
// // // 	return NULLPTR;
// // // }

// // // void * searchForSpace(int size, struct BuddyAllocator myAllocation) {
// // // 	if(myAllocation.size < size) {		// You will never have enough memory for this process.
// // // 		return NULLPTR;					// TODO: Should we consider a special msg?
// // // 	}
// // // 	return searchForSpaceDFS(size, myAllocation->root);
// // // }

// // // void * searchForSpaceDFS(int size, struct BuddyTreeNode treeNode) {
// // // 	void * newSpace;
// // // 	if((treeNode.block->size / 2) > size) {
// // // 		if(isChildFree(treeNode, treeNode->left)) {
// // // 			newSpace = searchForSpaceDFS(size, treeNode->left);
// // // 		} else {
// // // 			if(isChildFree(treeNode, treeNode->right)) {
// // // 				newSpace = searchForSpaceDFS(size, treeNode->right);
// // // 			}
// // // 		}
// // // 		if(newSpace != NULLPTR) {
// // // 			treeNode.block->free = SEMIFREE;		// TODO: HOJAS NO PUEDEN ESTAR SEMIFREE
// // // 		}
// // // 	} else {
// // // 		// preguntar ischildfree para saber si los hijos left & right tienen espacio o estan ocupados en su totalidad o parcialmente
// // // 		if((isChildFree(treeNode, treeNode->left) == 2) && (isChildFree(treeNode, treeNode->right) == 2)) {
// // // 			treeNode.block->free = OCCUPIED;
// // // 		}
// // // 		// hacer un switch en el que si estan libres "reserve" ese lugar
// // // 	}
// // // 	return newSpace;
// // // }

// // // char isChildFree(struct BuddyTreeNode parent, struct BuddyTreeNode child) {
// // // 	if(child == NULLPTR) {
// // // 		if(isNotLeaf(parent.block.size / 2)) {
// // // 			// "segmentar" memoria para crear un nuevo hijo treeNode
// // // 			// marcar los nietos con NULLPTR
// // // 			// linkearle los hijos al padre
// // // 			// marcar free = FREE
// // // 		} else {
// // // 			// marcar free = OCCUPIED
// // // 		}
// // // 	}
// // // 	return child.free;
// // // }

// // // char isNotLeaf(int size) {
// // // 	if(size < MINSIZE) {
// // // 		return 0;
// // // 	}
// // // 	return 1;
// // // }

// // int freePage(void * page) {
// // 	return 0;
// // }

// #include "memoryAllocation.h"
// #include <stdlib.h>
// #include <stdio.h>
// #include <math.h>


// struct node ** memoryBase;
// int totalLevels; 

// /*arreglar que por alguna razon el memoryBase value se cambia*/

// void startHeap(){

// 	memoryBase = malloc(sizeof(struct node*)*MEMORYPICES); 

// 	int elementNumber; 
// 	int level = 0; 
// 	int elementsInLevel;
// 	int j; 
// 	int nodoNumber; 

// 	for(elementNumber = 0; elementNumber<MEMORYPICES;){

// 		elementsInLevel = pow(2, level); 
// 		nodoNumber = 0; 

// 		for(j = elementsInLevel; j>0; j--){
// 			struct node *actualNode = (struct node*)malloc(sizeof(struct node)); 
// 			printf("%d\n",nodoNumber);
// 			fillInformation(actualNode, level, elementNumber, elementsInLevel, nodoNumber++); 
// 			printf("%p\n",memoryBase); 
// 			printf("level: %d elementNumber: %d elementsInLevel: %d nodoNumber: %d \n actualNode->level %d actualNode->state %d actualNode->base %p\n-----------------------------\n",level, elementNumber, elementsInLevel, nodoNumber, actualNode->level, actualNode->state, &actualNode->base);

// 			memoryBase[elementNumber++] = actualNode;
// 		}

// 		level++; 	
// 	}
// 	totalLevels = level; 
// }

// void fillInformation (struct node * nodeToFill, int level, int elementNumber, int elementsInLevel, int nodoNumber){

// 	nodeToFill->level = level; 
// 	nodeToFill->state = EMPTY; 
// 	nodeToFill->base = memoryBase + (nodoNumber*(MEMORYSIZE/elementsInLevel));

// }


// int getLevel(int size){  // por ejemplo si ingreso 75 puedo probar a partir de 2^7 y por lo tanto eso es a partir del 3 nivel [nivel 0 = 1024, nivel 1 = 512, nivel 3 = 128]

// 	int level = 1; 


// 	while(size > 0){
// 		size -= pow(2,level++);  
// 	}

// 	return totalLevels - level; 
// }

// void * allocPage (int size){ //indice<level es que es mas grande 

// 	if(size == 0 || size > PAGE_SIZE){
// 		return NULL;  
// 	}

// 	int level = getLevel(size); 

// 	if(level > totalLevels){
// 		return NULL; 
// 	}

// 	void * goHere = findSpace(level, 0, 1); 

// 	return goHere;
// }

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


// int main(int argc, char const *argv[])
// {
// 	startHeap(); 
// 	printf("totalLevels %d\n", totalLevels);
// 	printf("getLevel(75) %d \n", getLevel(75));
// 	return 0;
// }