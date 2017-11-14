#ifndef _ASYNCHRONOUSMP_H
#define _ASYNCHRONOUSMP_H

#include "queue.h"
#include "process.h"

void initMutex();

void deleteMutex();

int * getBolt();

int asyncSend(char * message, int destination_pid);

char * asyncReceive();

#endif