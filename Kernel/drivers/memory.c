#include <drivers.h>
/**
 * Esta driver es el que se ocupa de manejo de memoria
 */

/** Dummy malloc */
static void* memoryStart;

void initMemory() {
	memoryStart = allocNPages(100);
}

void *malloc(int size){
	void *ret = memoryStart;
	memoryStart += size;
	return ret;
}

void *calloc(int size){
	char* ret = malloc(size);
	for (int i = 0; i < size; i++)
		ret[i] = 0;
	return (void *)ret;
}

void free(void *memoryStart){
	return;
}