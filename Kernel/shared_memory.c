#include "shared_memory.h"

int createSharedMemory(int shared_memory_id, int pages) {
	struct process * current_process = getCurrentProcess();

	if (shared_memory_id == 0)
		return 0;
	
	void * ptr;
	if (getSharedMemory(shared_memory_id, &ptr) != 0) {
		return 0;
	}

	current_process->sm->id = shared_memory_id;
	current_process->sm->pointer = allocNPages(pages);

	return 1;
}

int getSharedMemory(int shared_memory_id, void **ptr) {
	struct process_node * current = getCurrentNode();


	int startingPid = current->process->pid;
	current = current->next;

	while (current->process->sm->id != shared_memory_id && current->process->pid != startingPid) {
		current = current->next;
	}

	if (current->process->pid == startingPid && current->process->sm->id != shared_memory_id){ 
		return 0;
	}

	*(ptr) = current->process->sm->pointer;

	return 1;
}

void freeSharedMemory(int shared_memory_id) {
	struct process_node * current = getCurrentNode();

	int startingPid = current->process->pid;
	current = current->next;

	while (current->process->sm->id != shared_memory_id && current->process->pid != startingPid) {
		current = current->next;
	}

	if (current->process->pid == startingPid && current->process->sm->id != shared_memory_id) 
		return;

	current->process->sm->id = 0;
	freeSpace(current->process->sm->pointer);
}



