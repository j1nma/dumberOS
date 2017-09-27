#include <drivers.h>
/**
 * Esta driver es el que se ocupa de manejo de memoria.
 */

static void* ptr = 0x600000;

void *malloc(int size){
	void *ret = ptr;
	ptr += size;
	return ret;
}

void *calloc(int size){
	char* ret = malloc(size);
	for (int i = 0; i < size; i++)
		ret[i] = 0;
	return (void *)ret;
}

void free(void *ptr){
	return;
}