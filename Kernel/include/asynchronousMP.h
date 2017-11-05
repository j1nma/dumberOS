#ifndef _ASYNCHRONOUSMP_H
#define _ASYNCHRONOUSMP_H

#include "queue.h"
#include "circular_buffer.h"
#include "process.h"

void mutex_down();

void mutex_up();

int peekWaitQueue(struct process * receiver);

void pushWaitQueue(struct process * receiver, struct process * sender);

int popWaitQueue(struct process * process);

void asyncSend(char * message, int destination_pid);

char * asyncReceive();

#endif