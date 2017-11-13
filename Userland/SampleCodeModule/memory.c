// #include <stdio.h>
#include "include/memory.h"
// #include <stdlib.h>
#include <../../../Kernel/include/syscalls.h>

Header * first = NULL;
Header base;	// me sirve para asignar un espacio de memoria a mi lista

void * mymalloc(unsigned nBytes) {
	// Header * aux = first;
	// int memBlockAvail = 0;

	// while(!memBlockAvail) {
	// 	if(aux == NULL) {
	// 		Header * newBlock = morecore(nBytes);
	// 	}
	// }
	Header * aux, *prev;
	// Header * morecore(unsigned);

	// unsigned nUnits = (nBytes + sizeof(Header)-1)/sizeof(Header)+1; /*TODO: VOLVER A CAMBIAR ESTO - cuentita;*/
	unsigned nUnits = nBytes;

	// si malloc no fue llamada nunca => first = NULL, tengo que inicializarlo, como no puedo usar malloc, le asigno una posicion cualquiera = base
	if((prev = first) == NULL) {
		base.availableSize = 0;
		base.unmutableSize = 0;
		base.next = first = prev = &base; // first = prev son iguales al inicio porque la lista es una lista circular, facilita la funcion
	}

	// el for me soluciona el tema de que si llegue al final y reserve espacio de mas, puedo volver a "solucionarlo"

	for(aux = prev->next; ;prev = aux, aux = aux->next) {
		if(aux->availableSize >= nUnits) {
			if(aux->availableSize == nUnits) {
				prev->next = aux->next;
				aux->next = NULL;
			} else {			// it's big enough to fit and have spare, going to divide the block
				aux->availableSize -= nUnits;
				aux += aux->availableSize;
				aux->availableSize = nUnits;
			}
			first = prev; /*para mantener la circularidad*/
			return (void*)(aux + 1);
		}
		if(aux == first) {		// moved through the whole list and nothing was big enough to fit nBytes
			if((aux = morecore(nBytes)) == NULL) {
				return NULL;	// nothing free, come back later
			}
		}
	}
}


// void myfree(void *toFree) {
Header* myfree(void *toFree) {
	Header *bp, *aux;
	bp = (Header *)toFree - 1; /*apunta al encabezador de un bloque*/
	aux = first; 
	for((aux = first); !((bp > aux) && (bp < aux->next)); (aux=aux->next))
		if(aux >= aux->next && (bp > aux || bp < aux->next)){
			break;
		}
		if(bp + (bp -> availableSize) == aux->next){
			bp -> availableSize += aux->next->availableSize; 
			bp->next = aux->next->next; 
		} else
			bp -> next = aux-> next;
		if(aux+(aux->availableSize) == bp) {
			aux->availableSize += bp->availableSize;
			if(aux->availableSize == aux->unmutableSize) {
				// syscall(SYSCALL_FREE, bp, 0, 0);
				free(bp);
			}
			aux->next = bp->next;
		} else
			aux->next = bp;
	first = aux;
	return aux;
}

/*
** Requests more memory to the OS.
** Sees that a new memory block is supplied.
*/
Header * morecore(unsigned nBytes) {
	// Header * newBlock;
	// unsigned auxSize = 0;

	// if(nBytes <= PAGE_SIZE) {
	// 	newBlock = malloc(sizeof(Header));
	// 	newBlock->base = malloc(nBytes);
	// }
	Header * newBlock;
	unsigned auxSize = 0;

	if(nBytes <= PAGE_SIZE) {
		nBytes = 1;
	} else {
		auxSize = nBytes/PAGE_SIZE + 1;
		nBytes = auxSize;
	}
	// newBlock = (Header *) syscall(SYSCALL_MALLOC, 0, nBytes, 0); /*allocPage(nBytes)*/
	newBlock = malloc(nBytes*PAGE_SIZE);

	if(newBlock == NULL) {
		return NULL;
	}

	newBlock->availableSize = PAGE_SIZE;
	newBlock->unmutableSize = newBlock->availableSize;

	Header *aux;
	aux = first;

	myfree((void*)(newBlock + 1));
	// return newBlock;
	return first;
}


// int main(int argc, char const *argv[])
// {
// 	// void * trial2 = mymalloc(2);
// 	// printf("respuesta2 %p\n", trial2);

// 	// void * trial3 = mymalloc(3);
// 	// printf("respuesta3 %p\n", trial3);
	
// 	// void * trial4 = mymalloc(5); 
// 	// printf("respuesta5 %p\n", trial4);
	
// 	// void * trial5 = mymalloc(100); 
// 	// printf("respuesta100 %p\n", trial5);
	
// 	// void * trial6 = mymalloc(190);
// 	// printf("respuesta190 %p\n", trial6);
	
// 	// void *trial7 = mymalloc(10); 
// 	// printf("respuesta10 %p\n", trial7);
	
// 	// myfree(trial2); 
// 	// printf("respuesta2 post free %p\n",trial2);

// 	printf("Hago el malloc\n");
// 	void * mymemory = malloc(sizeof(1024*1024));
// 	printf("Comienzo con los mymalloc\n");
// 	void * test1 = mymalloc(1500);
// 	void * test3 = mymalloc(1500);
// 	void * test2 = mymalloc(800);
// 	printf("Test1 base: %p\n", test1);
// 	printf("Test2 base: %p\n", test2);
// 	printf("Test3 base: %p\n", test3);
// 	void * test4 = mymalloc(1024*1024);				// infinite loop here
// 	printf("Ya se hizo el 4to malloc\n");
// 	void * test5 = mymalloc(1500);
// 	printf("Test4 base: %p\n", test4);
// 	printf("Test5 base: %p\n", test5);
// 	void * test6 = mymalloc(1500);
// 	void * test7 = mymalloc(1500);
// 	void * test8 = mymalloc(1500);
// 	void * test9 = mymalloc(1500);
// 	void * test10 = mymalloc(131071);
// 	void * test11 = mymalloc(131072);
// 	void * test12 = mymalloc(131073);
// 	void * test13 = mymalloc(131071);
// 	void * test14 = mymalloc(131071);
// 	void * test15 = mymalloc(131071);
// 	void * test16 = mymalloc(1500);
// 	void * test17 = mymalloc(1500);
// 	void * test18 = mymalloc(1500);
// 	void * test19 = mymalloc(1500);
// 	void * test20 = mymalloc(1500);
// 	void * test21 = mymalloc(1500);
// 	void * test22 = mymalloc(1500);
// 	void * test23 = mymalloc(1500);
// 	void * test24 = mymalloc(1500);
// 	void * test25 = mymalloc(131071);
// 	printf("Test10 base: %p\n", test10);
// 	printf("Test11 base: %p\n", test11);
// 	printf("Test12 base: %p\n", test12);
// 	printf("Test13 base: %p\n", test13);
// 	printf("Test14 base: %p\n", test14);
// 	printf("Test15 base: %p\n", test15);
// 	printf("Test16 base: %p\n", test16);
// 	printf("Test25 base: %p\n", test25);
// 	// printBuddy();
// 	printf("Freeing fake page: %p\n", myfree(test1+2));
// 	// printBuddy();
// 	printf("Freeing test1: %p\n", myfree(test1));
// 	// printBuddy();
// 	printf("Freeing test2: %p\n", myfree(test2));
// 	printf("Freeing test3: %p\n", myfree(test3));
// 	// printBuddy();
// 	printf("Freeing test4 (should not work): %p\n", myfree(test4));
// 	printf("Freeing test5: %p\n", myfree(test5));
// 	printf("Freeing test6: %p\n", myfree(test6));
// 	printf("Freeing test7: %p\n", myfree(test7));
// 	printf("Freeing test8: %p\n", myfree(test8));
// 	printf("Freeing test9: %p\n", myfree(test9));
// 	printf("Freeing test10: %p\n", myfree(test10));
// 	// printBuddy();
// 	printf("Freeing test11: %p\n", myfree(test11));
// 	printf("Freeing test12: %p\n", myfree(test12));
// 	printf("Freeing test13: %p\n", myfree(test13));
// 	printf("Freeing test14: %p\n", myfree(test14));
// 	printf("Freeing test15: %p\n", myfree(test15));
// 	printf("Freeing test16: %p\n", myfree(test16));
// 	printf("Freeing test17: %p\n", myfree(test17));
// 	printf("Freeing test18: %p\n", myfree(test18));
// 	printf("Freeing test19: %p\n", myfree(test19));
// 	printf("Freeing test20: %p\n", myfree(test20));
// 	printf("Freeing test21: %p\n", myfree(test21));
// 	printf("Freeing test22: %p\n", myfree(test22));
// 	printf("Freeing test22: %p\n", myfree(test22));
// 	printf("Freeing test23: %p\n", myfree(test23));
// 	printf("Freeing test24: %p\n", myfree(test24));
// 	printf("Freeing test25: %p\n", myfree(test25));
// 	// printBuddy();

// 	free(mymemory);

// 	// printf("%d\n", sizeof(char));
	
// 	return 0;
// }
