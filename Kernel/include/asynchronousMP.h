#ifndef _ASYNCHRONOUSMP_H
#define _ASYNCHRONOUSMP_H

#include "queue.h"
#include "circular_buffer.h"
#include "process.h"

void mutex_down();

void mutex_up();

int peekWaitQueue(Queue * sender_waiting_processes);

void pushWaitQueue(Queue * sender_waiting_processes, int sender_pid);

int popWaitQueue(Queue * sender_waiting_processes);

void asyncSend(char * message, int destination_pid);

char * asyncReceive();

#endif