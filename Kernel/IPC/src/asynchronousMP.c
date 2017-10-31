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

void unblockProcess(struct process * process) {
  process->state = MESSAGE_UNBLOCK;
}

int isBlocked(struct process * process) {
  return process->state == MESSAGE_BLOCK;
}

int topWaitQueue(struct process * process) {
  Queue q = process->blocked_waiting_processes;

  int ans;

  queuePeek(&q, &ans);

  return ans;
}

void pushWaitQueue(struct process * receiver, struct process * sender) {
  enqueue(&receiver->blocked_waiting_processes, &sender->pid);
}


int popWaitQueue(struct process * process) {
  Queue q = process->blocked_waiting_processes;

  int ans;

  dequeue(&q, &ans);

  return ans;
}

static circular_buffer buff;

static struct process * current_process;

void asyncSend(struct message * msg) {

  disableTaskSwitch();

  msg->source = current_process; //we must not rely on it's set

  // circular_buffer tmpbuff = map_buffer(msg->destination); //temporarily map destination's buffer into sender process' address space

  circular_buffer tmpbuff;

  cbInit(&tmpbuff, sizeof(struct message));

  tmpbuff = msg->destination->blocked_waiting_processes; //temporarily map destination's buffer into sender process' address space

  // if (tmpbuff.count == MAXITEMS) { //if receiver buffer is full, block
  //   pushwaitqueue(msg.dst, current_process); //record this process in dst's sender queue
  //   block(current_process);
  // }
  // push(tmpbuff, msg);
  // if (isblocked(msg.dst)) awake(msg.dst); //if destination process is blocked for receiving, awake it
  // unmap_buffer();

  enableTaskSwitch();
}

struct message * asyncRecieve() {
  // message tmp=NULL;
  // disable_task_switch();
  // if (buff.count==0) block(current_process); //if there's nothing to get, block
  // tmp=pop(buff);
  // while(topwaitqueue()!=NULL) awake(popwaitqueue()); //awake blocked processes waiting to send
  // enable_task_switch();
  // return (tmp);
}