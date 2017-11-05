#ifndef _CIRCULAR_BUFFER_H_
#define _CIRCULAR_BUFFER_H_

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include "message.h"

#define MAXITEMS 1
#define MESSAGE_SIZE 240

typedef struct circular_buffer {

	char * * buffer; //buffer to hold messages

	char * buffer_end; // end of data buffer
	size_t count;     // number of items in the buffer
	size_t sz;        // size of each item in the buffer
	char * head;       // pointer to head
	char * tail;       // pointer to tail

} circular_buffer;

void cbInit(circular_buffer * cb, size_t sz);

void cbFree(circular_buffer * cb);

void cbPushBack(circular_buffer * cb, const char * item);

void cbPopFront(circular_buffer * cb, char * item);


#endif