#include "message.h"
#include "circular_buffer.h"

#define UNLOCKED 0
#define LOCKED 1

// long lock = UNLOCKED;

// void Acquire(void) {
//    while(lock == LOCKED);
//    lock = LOCKED;
// }   Acquire 

// void Relinquish(void) {
//    lock = UNLOCKED;
// }  /* Relinquish */

void asyncSend(struct message * msg);

struct message * asyncRecieve();

void disableTaskSwitch();

void enableTaskSwitch();

void wait_and_lock_mutex();

void unlock_mutex();

void disableTaskSwitch() {

}

void enableTaskSwitch() {

}

void wait_and_lock_mutex() {

}

void unlock_mutex() {

}

void asyncSend(struct message * msg) {
  disableTaskSwitch();
  // msg.src = current_process; //we must not rely on it's set
  // tmpbuff = map_buffer(msg.dst); //temporarily map destination's buffer into sender process' address space
  // if (tmpbuff.count == MAXITEMS) { //if receiver buffer is full, block
  //   pushwaitqueue(msg.dst, current_process); //record this process in dst's sender queue
  //   block(current_process);
  // }
  // push(tmpbuff, msg);
  // if (isblocked(msg.dst)) awake(msg.dst); //if destination process is blocked for receiving, awake it
  // unmap_buffer();
  enableTaskSwitch();
}

static circular_buffer buff;

struct message * asyncRecieve() {
  // message tmp=NULL;
  // disable_task_switch();
  // if (buff.count==0) block(current_process); //if there's nothing to get, block
  // tmp=pop(buff);
  // while(topwaitqueue()!=NULL) awake(popwaitqueue()); //awake blocked processes waiting to send
  // enable_task_switch();
  // return (tmp);
}