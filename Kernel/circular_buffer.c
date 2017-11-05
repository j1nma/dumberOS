#include <sys/types.h>
#include <stdio.h>
#include "circular_buffer.h"
#include "drivers.h"
#include "lib.h"

/*
Based on https://stackoverflow.com/a/827749/1815727 27, October 2017 02:56 AM
*/

void cbInit(circular_buffer * cb, size_t sz) {
    cb->buffer = malloc(MAXITEMS * sz);

    if (cb->buffer == NULL)
        cb->buffer_end = (char *)cb->buffer + MAXITEMS * sz;

    cb->count = 0;
    cb->sz = sz;
    cb->head = cb->buffer;
    cb->tail = cb->buffer;
}

void cbFree(circular_buffer * cb) {
    free(cb->buffer);
    // clear out other fields too, just to be safe
}

void cbPushBack(circular_buffer * cb, const char * item) {
    if (cb->count == MAXITEMS) {
        // handle error
        return;
    }

    myMemCpy(cb->head, item, cb->sz);
    // strcpy(cb->head, item, cb->sz); 
    cb->head = cb->head + cb->sz;

    if (cb->head == cb->buffer_end)
        cb->head = cb->buffer;

    cb->count++;
}

void cbPopFront(circular_buffer * cb, char * item) {
    if (cb->count == 0) {
        // handle error
        return;
    }
    write("premem\n", 7);
    myMemCpy(item, cb->tail, cb->sz);
    // strcpy(item, cb->tail);
    write("postmem\n", 8);
    // write(item, 6);

    cb->tail = cb->tail + cb->sz;
    if (cb->tail == cb->buffer_end)
        cb->tail = cb->buffer;
    cb->count--;
}