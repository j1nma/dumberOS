#include "stack.h"

#define SIZE 200

int MAXSIZE = SIZE;
void * stack[SIZE];
int top = -1;

int isEmpty() {

   if (top == -1)
      return 1;
   else
      return 0;
}

int isFull() {

   if (top == MAXSIZE)
      return 1;
   else
      return 0;
}

void * peek() {
   return stack[top];
}

void * pop() {
   void * data;

   if (!isEmpty()) {
      data = stack[top];
      top = top - 1;
      return data;
   }

   return 0;
}

void push(void * data) {

   if (!isFull()) {
      top = top + 1;
      stack[top] = data;
   }
}