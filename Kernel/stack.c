#include "stack.h"

int MAXSIZE = 8;       
void * stack[8];     
int top = -1;            

int isempty() {

   if(top == -1)
      return 1;
   else
      return 0;
}
   
int isfull() {

   if(top == MAXSIZE)
      return 1;
   else
      return 0;
}

void * peek() {
   return stack[top];
}

void * pop() {
   void * data;
	
   if(!isempty()) {
      data = stack[top];
      top = top - 1;   
      return data;
   } else {
      
   }
}

void push(void * data) {

   if(!isfull()) {
      top = top + 1;   
      stack[top] = data;
   } else {
      
   }
}