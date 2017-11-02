#include <stdlib.h>
#include "message.h"
#include "mutex.h"
#include "process.h"
#include "circular_buffer.h"
#include "queue.h"

void disableTaskSwitch() {
  down();
}

void enableTaskSwitch() {
  up();
}

void blockProcess(struct process * process) {
  process->state = MESSAGE_BLOCK;
}

void awakeProcess(struct process * process) {
  process->state = MESSAGE_UNBLOCK;
}

int isBlocked(struct process * process) {
  return process->state == MESSAGE_BLOCK;
}

int peekWaitQueue(struct process * process) {
  Queue q;
  queueInit(&q, sizeof(int));

  q = process->sender_waiting_processes;

  int ans = -1;
  queuePeek(&q, &ans);

  return ans;
}

void pushWaitQueue(struct process * receiver, struct process * sender) {
  Queue q;
  queueInit(&q, sizeof(int));

  q = receiver->sender_waiting_processes;

  enqueue(&q, &(sender->pid));
}


int popWaitQueue(struct process * process) {
  Queue q;
  queueInit(&q, sizeof(int));

  q = process->sender_waiting_processes;

  int ans;
  dequeue(&q, &ans);

  return ans;
}

static struct process * current_process;

/*
Now let's start with sending a message, and not care about. This could lead to losing a message, 
which we can't afford, so we'll do a trick here.
Despite of being asynchronous, we will block if receiver buffer is full,
and we'll continue only after there's space for our message.
*/
void asyncSend(struct message * msg) {

  disableTaskSwitch();

  msg->source = current_process; //we must not rely on it's set

  circular_buffer * tmpbuff;
  cbInit(tmpbuff, sizeof(struct message));

  tmpbuff = msg->destination->receiver_buffer; //temporarily map destination's buffer into sender process' address space

  if (tmpbuff->count == MAXITEMS) { //if receiver buffer is full, block
    pushWaitQueue(msg->destination, current_process); //record this process in dst's sender queue
    blockProcess(current_process);
  }

  cbPushBack(tmpbuff, msg);
  if (isBlocked(msg->destination)) awakeProcess(msg->destination); //if destination process is blocked for receiving, awake it
  cbFree(tmpbuff); // unmap_buffer();

  enableTaskSwitch();

}

/*
Doesn't matter whether it's synchronized or not,
receiver must block if it's message queue is empty,
and there's nothing to process.
*/
struct message * asyncRecieve(struct process * current_process, circular_buffer * cb) {
  
  struct message * tmp;

  disableTaskSwitch();

  if (cb->count == 0) blockProcess(current_process); //if there's nothing to get, block

  cbPopFront(cb, tmp);
 

  while (peekWaitQueue()! = -1) awakeProcess(popWaitQueue()); //awake blocked processes waiting to send

  enableTaskSwitch();

  return tmp;
}