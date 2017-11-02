#ifndef _ASYNCHRONOUSMP_H
#define _ASYNCHRONOUSMP_H

#include "queue.h"
#include "circular_buffer.h"

void disableTaskSwitch();

void enableTaskSwitch();

int peekWaitQueue(struct process * process);

void pushWaitQueue(struct process * receiver, struct process * sender);

int popWaitQueue(struct process * process);

void asyncSend(char * message, int destination_pid);

char * asyncRecieve();

#endif