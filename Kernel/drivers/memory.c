// #include <drivers.h>
// /**
//  * Esta driver es el que se ocupa de manejo de memoria.
//  */

// static void* ptr = 0x600000;

// void *malloc(int size){
// 	void *ret = ptr;
// 	ptr += size;
// 	return ret;
// }

// void *calloc(int size){
// 	char* ret = malloc(size);
// 	for (int i = 0; i < size; i++)
// 		ret[i] = 0;
// 	return (void *)ret;
// }

// void free(void *ptr){
// 	return;
// }

/* El Lenguaje de Programacion C - Brian W. Kernighan & Denis M. Ritchie - Paginas 205 a la 208 */ 
#include <stdio.h>
#include "../include/memoryAllocation.h"

Header * first = NULL;
Header base;	// me sirve para asignar un espacio de memoria a mi lista

void * memAlloc(unsigned nBytes){
	
	Header * aux, *prev; 
	Header * morecore(unsigned); 

	unsigned nUnits = (nBytes + sizeof(Header)-1)/sizeof(Header)+1; /*TODO: VOLVER A CAMBIAR ESTO - cuentita;*/

	/*si malloc no fue llamada nunca => first = NULL, tengo que inicializarlo, como no puedo usar malloc, le asigno una posicion cualquiera = base */
	if((prev = first) == NULL) {
		base.availableSize = 0;
		base.unmutableSize = 0;
		base.next = first = prev = &base; /* first = prev son iguales al inicio porque la lista es una lista circular, facilita la funcion*/
	}

	/*el for me soluciona el tema de que si llegue al final y reserve espacio de mas, puedo volver a "solucionarlo"*/

	for(aux = prev->next; ;prev = aux, aux = aux->next) {
		if(aux->availableSize >= nUnits) {
			if(aux->availableSize == nUnits) {
				prev->next = aux->next;
				aux->next = NULL;
			} else { /*es mas grande voy a tener que dividir el bloque*/
				aux->availableSize -= nUnits;
				aux += aux->availableSize;
				aux->availableSize = nUnits;
			}
			first = prev; /*para mantener la circularidad*/
			return (void*)(aux + 1);
		}
		 if(aux == first) { /*di la vuelta completa a la lista circular*/
		 	if((aux = morecore(nBytes)) == NULL) {
		 		return NULL;  /*nada libre*/
		 	}
		 }
	}
}


void memFree(void *toFree){
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
				freePage(bp);
			}
			aux->next = bp->next;
		} else
			aux->next = bp;
	first = aux;
}

/*
** Requests more memory to the OS.
** Sees that a new memory block is supplied.
*/
Header * morecore(unsigned nBytes){
	Header * newBlock; 

	if(nBytes < SMALLESTBLOCKSIZE) {
		nBytes = SMALLESTBLOCKSIZE;
	}

	newBlock = (Header *) allocSpace(SMALLESTBLOCKSIZE); /*allocPage(nBytes)*/

	if(newBlock == NULL){
		return NULL; 
	}

	newBlock->availableSize = SMALLESTBLOCKSIZE;
	newBlock->unmutableSize = newBlock->availableSize;
	
	Header *aux; 
	aux = first; 

	memFree((void*)(newBlock + 1)); 
	return newBlock; 
}


// int main(int argc, char const *argv[])
// {
// 	void * trial2 = malloc(2);
// 	printf("respuesta2 %p\n",trial2);
// 	void * trial3 = malloc(3);
// 	printf("respuesta3 %p\n",trial3);
// 	void * trial4 = malloc(5); 
// 	printf("respuesta4 %p\n",trial4); 
// 	void * trial5 = malloc(100); 
// 	printf("respuesta5 %p\n",trial5 );
// 	void * trial6 = malloc(190);
// 	printf("respuesta6 %p\n",trial6);
// 	void *trial7 = malloc(10); 
// 	printf("respuesta7 %p\n",trial7);
// 	free1(trial2); 
// 	printf("respuesta2 post free %p\n",trial2);
// 	return 0;
// }
