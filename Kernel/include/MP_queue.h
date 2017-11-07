#ifndef _MP_QUEUE_H_INCLUDED
#define _MP_QUEUE_H_INCLUDED

#include <stddef.h>

// Copyright https://codereview.stackexchange.com/questions/141238/implementing-a-generic-queue-in-c

typedef struct Node {
	void * data;
	struct Node * next;
} node;

typedef struct QueueList {
	int sizeOfQueue;
	size_t memSize;
	node * head;
	node * tail;
} Queue;

void queueInit(Queue * q, size_t memSize);

int enqueue(Queue * q, const void *);

void dequeue(Queue * q, void *);

int queuePeek(Queue * q, void *);

void clearQueue(Queue * q);

int getQueueSize(Queue * q);

#endif