#ifndef _SHARED_MEM_H
#define _SHARED_MEM_H

#include "process_node.h"
#include "scheduler.h"
#include "process.h"
#include "memoryAllocation.h"

struct shared_mem {
	int id;
	void *pointer;
};

int createSharedMemory(int shared_memory_id, int pages);
int getSharedMemory(int shared_memory_id, void **ptr);
void freeSharedMemory(int shared_memory_id);

#endif