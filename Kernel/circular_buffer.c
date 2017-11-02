#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include "circular_buffer.h"
#include "drivers.h"

/*
Copied from https://stackoverflow.com/a/827749/1815727 27, October 2017 02:56 AM
*/

void cbInit(circular_buffer * cb, size_t sz) {
    cb->buffer = malloc(MAXITEMS * sz);
    if (cb->buffer == NULL)
        // handle error
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

void cbPushBack(circular_buffer * cb, const void * item) {
    if (cb->count == MAXITEMS) {
        // handle error
    }
    memcpy(cb->head, item, cb->sz);
    cb->head = (char*)cb->head + cb->sz;
    if (cb->head == cb->buffer_end)
        cb->head = cb->buffer;
    cb->count++;
}

void cbPopFront(circular_buffer * cb, void * item) {
    if (cb->count == 0) {
        // handle error
    }
    memcpy(item, cb->tail, cb->sz);
    cb->tail = (char*)cb->tail + cb->sz;
    if (cb->tail == cb->buffer_end)
        cb->tail = cb->buffer;
    cb->count--;
}