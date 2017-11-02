#ifndef _ASYNCHRONOUSMP_H
#define _ASYNCHRONOUSMP_H

#include "queue.h"
#include "circular_buffer.h"

void disableTaskSwitch();

void enableTaskSwitch();

void blockProcess(struct process * process);

void awakeProcess(struct process * process);

int isBlocked(struct process * process);

int peekWaitQueue(struct process * process);

void pushWaitQueue(struct process * receiver, struct process * sender);

int popWaitQueue(struct process * process);

void asyncSend(struct message * msg);

struct message * asyncRecieve(struct process * current_process, circular_buffer * cb);

#endif