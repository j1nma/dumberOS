#include <stdio.h>
#include <lib.h>
#include "queue.h"
#include "drivers.h"

// Copyright https://codereview.stackexchange.com/questions/141238/implementing-a-generic-queue-in-c

void queueInit(Queue * q, size_t memSize) {
  q->sizeOfQueue = 0;
  q->memSize = memSize;
  q->head = q->tail = NULL;
}

int enqueue(Queue * q, const void * data) {

  node * newNode = (node *)memAlloc(sizeof(node));

  if (newNode == NULL) {
    return -1;
  }

  newNode->data = memAlloc(q->memSize);

  if (newNode->data == NULL) {
    memFree(newNode);
    return -1;
  }

  newNode->next = NULL;

  memcpy(newNode->data, data, q->memSize);

  if (q->sizeOfQueue == 0) {
    q->head = q->tail = newNode;
  }
  else {
    q->tail->next = newNode;
    q->tail = newNode;
  }

  q->sizeOfQueue++;

  return 0;
}

void dequeue(Queue * q, void * data) {
  if (q->sizeOfQueue > 0) {

    node * temp = q->head;
    memcpy(data, temp->data, q->memSize);

    if (q->sizeOfQueue > 1) {
      q->head = q->head->next;
    }
    else {
      q->head = NULL;
      q->tail = NULL;
    }

    q->sizeOfQueue--;
    memFree(temp->data);
    memFree(temp);
  }
}

int queuePeek(Queue * q, void * data) {
  if (q->sizeOfQueue > 0) {
    node * temp = q->head;
    memcpy(data, temp->data, q->memSize);
    return 0;
  }

  return -1;
}

void clearQueue(Queue * q) {
  node * temp;

  while (q->sizeOfQueue > 0) {
    temp = q->head;
    q->head = temp->next;
    memFree(temp->data);
    memFree(temp);
    q->sizeOfQueue--;
  }

  q->head = q->tail = NULL;

  memFree(q);
}

int getQueueSize(Queue * q) {
  return q->sizeOfQueue;
}