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

    int i;

    for (i = 0; i < MAXITEMS; i++) {

        cb->buffer[i] = calloc((MESSAGE_SIZE + 1) * sizeof(char));

        if (cb->buffer[i] == NULL) {
            /* problem */
            write("Sin memoria\n.", 13);
            cbFree(cb);
            break;
        }
    }

    cb->count = 0;
    cb->sz = sz;
    cb->head = cb->buffer;
    cb->tail = cb->buffer;
}

void cbFree(circular_buffer * cb) {
    int i;

    for (i = 0; i < MAXITEMS; i++) {
        free(cb->buffer[i]);
    }

    free(cb->buffer);
}

void cbPushBack(circular_buffer * cb, const char * item) {
    if (cb->count == MAXITEMS) {
        // handle error
        return;
    }

    // memcpy(cb->head, item, cb->sz);
    strcpy(cb->head, item);
    
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

    // memcpy(item, cb->tail, cb->sz);
    strcpy(item, cb->tail);

    /* Copy the contents from tail to item */
    // int i = 0;

    // for (i = 0; i < 7; i++)
    //     item++;

    // *item++ = '\0';

    cb->tail = cb->tail + cb->sz;

    if (cb->tail == cb->buffer_end)
        cb->tail = cb->buffer;

    cb->count--;
}