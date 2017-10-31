#ifndef _CIRCULAR_BUFFER_H_
#define _CIRCULAR_BUFFER_H_

#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>


typedef struct circular_buffer {
	void *buffer;     // data buffer
	void *buffer_end; // end of data buffer
	size_t capacity;  // maximum number of items in the buffer
	size_t count;     // number of items in the buffer
	size_t sz;        // size of each item in the buffer
	void *head;       // pointer to head
	void *tail;       // pointer to tail

} circular_buffer;

void cbInit(circular_buffer *cb, size_t capacity, size_t sz);

void cbFree(circular_buffer *cb);

void cbPushBack(circular_buffer *cb, const void *item);

void cbPopFront(circular_buffer *cb, void *item);


#endif