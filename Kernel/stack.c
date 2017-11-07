#include "stack.h"

int MAXSIZE = 50;
void * stack[50];
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