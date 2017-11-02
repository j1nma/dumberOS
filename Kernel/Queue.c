#include "Queue.h"

#define MAX 50

void* intArray[MAX];
int front = 0;
int rear = -1;
int itemCount = 0;

void* queuePeek() {
   return intArray[front];
}

int isQueueEmpty() {
   return itemCount == 0;
}

int isQueueFull() {
   return itemCount == MAX;
}

int queueSize() {
   return itemCount;
}  

void queueInsert(void* data) {

   if(!isQueueFull()) {
  
      if(rear == MAX-1) {
         rear = -1;            
      }       

      intArray[++rear] = data;
      itemCount++;
   }
}

void* queueRemove() {
   void* data = intArray[front++];
  
   if(front == MAX) {
      front = 0;
   }
  
   itemCount--;
   return data;  
}