#ifndef _ASYNCHRONOUSMP_H
#define _ASYNCHRONOUSMP_H

#include "MP_queue.h"
#include "process.h"

void mutex_down();

void mutex_up();

void asyncSend(char * message, int destination_pid);

char * asyncReceive();

#endif