#include <naiveConsole.h>
#include <drivers.h>
/**
 * Esta driver es el que se ocupa de manejo de memoria.
 */

static void* ptr = 0x600000;
static int pepito = 0;

void * mymalloc(int size){
	void *ret = ptr;
	ptr += size;
	return ret;
}

void *calloc(int size){
	char* ret = mymalloc(size);
	for (int i = 0; i < size; i++)
		ret[i] = 0;
	return (void *)ret;
}

void myfree(void *ptr){
	return;
}

#define SMALLBLOCKSIZE 1024
#define MYNULL 0x0

typedef struct header {
	unsigned availableSize;
	struct header * next;
} Header;

Header * first = NULL;
Header base;

void * malloc(unsigned nBytes) {
	Header * aux;
	Header * prev;
	Header * morecore(unsigned);

	unsigned nUnits = nBytes;

	if((prev = first) == MYNULL) {
		base.availableSize = 0;
		base.next = first = prev = &base;
	}

	for(aux = prev->next; ; prev = aux, aux = aux->next) {
		if(aux->availableSize >= nUnits) {
			if(aux->availableSize == nUnits) {
				prev->next = aux->next;
			} else {
				aux->availableSize -= nUnits;
				aux += aux->availableSize;
				aux->availableSize = nUnits;
			}
			first = prev;
			ncPrint("hola");
			return (void*)(aux + 1);
		}

		if(aux == first) {
			if((aux = morecore(nUnits)) == MYNULL) {
				return NULL;
			}
		}
	}
}

Header * morecore(unsigned nBytes) {
	Header * newBlock;
	void * cp;

	if(nBytes < SMALLBLOCKSIZE) {
		nBytes = SMALLBLOCKSIZE;
	}
	cp = mymalloc(nBytes * sizeof(Header));
	if(cp == MYNULL) {
		return MYNULL;
	}
	newBlock = (Header*) cp;
	newBlock->availableSize = nBytes;
	ncPrintHex(first);
	ncPrint("antes del free");
	ncNewline();
	free((void*)(newBlock+1));
	ncPrintHex(first);
	ncPrint("antes del return");
	ncNewline();
	return first;
}

void free(void * toFree) {
	Header * bp;
	Header * aux;

	ncPrint("entre a free: ");
	ncPrintDec(pepito);
	pepito++;
	ncNewline();

	bp = (Header*) (toFree-1);

	for(aux = first; !(bp > aux && bp < aux->next); aux = aux->next)
		if(aux >= aux->next && (bp > aux || bp < aux->next)) {
			break;
		}

	if(bp + bp->availableSize == aux->next) {
		bp->availableSize += aux->next->availableSize;
		bp->next = aux->next->next;
	} else {
		bp->next = aux->next;
	}

	if(aux + aux->availableSize == bp) {
		aux->availableSize += bp->availableSize;
		aux->next = bp->next;
	} else {
		aux->next = bp;
	}
	first = aux;
}
