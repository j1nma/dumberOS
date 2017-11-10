#ifndef _ASYNCHRONOUSMP_H
#define _ASYNCHRONOUSMP_H

#include "MP_queue.h"
#include "process.h"

void initMutex();

void deleteMutex();

int * getBolt();

void asyncSend(char * message, int destination_pid);

char * asyncReceive();

#endif