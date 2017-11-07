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
  if(!q->sizeOfQueue) write("No habia nada.\n", 16);

  if(q->sizeOfQueue == 1) write("Hay solo uno.\n", 15);

  if(q->sizeOfQueue >= 1) write("Hay mas uno.\n", 14);

  node * newNode = (node *)malloc(sizeof(node));

  if (newNode == NULL) {
    return -1;
  }

  newNode->data = malloc(q->memSize);

  if (newNode->data == NULL) {
    free(newNode);
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

  if(!(q->sizeOfQueue)) write("Ahora no hay nada.\n", 20);

  if(q->sizeOfQueue == 1) write("Ahora hay solo uno.\n", 21);

  if(q->sizeOfQueue == 2) write("Ahora hay dos.\n", 16);

  if(q->sizeOfQueue == 3) write("Ahora hay tres.\n", 17);
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
    free(temp->data);
    free(temp);
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
    free(temp->data);
    free(temp);
    q->sizeOfQueue--;
  }

  q->head = q->tail = NULL;

  free(q);
}

int getQueueSize(Queue * q) {
  return q->sizeOfQueue;
}