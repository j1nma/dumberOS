// Copyright https://codereview.stackexchange.com/questions/141238/implementing-a-generic-queue-in-c

#ifndef _QUEUE_H_INCLUDED
#define _QUEUE_H_INCLUDED

#include <stddef.h>

typedef struct Node
{
	void *data;
	struct Node *next;
} node;

typedef struct QueueList
{
	int sizeOfQueue;
	size_t memSize;
	node *head;
	node *tail;
} Queue;

void queueInit(Queue *q, size_t memSize);
int enqueue(Queue *, const void *);
void dequeue(Queue *, void *);
void queuePeek(Queue *, void *);
void clearQueue(Queue *);
int getQueueSize(Queue *);

#endif