#ifndef PROCESS_NODE_H
#define PROCESS_NODE_H

#include "process.h"

struct process_node {

	struct process * process;
	
	struct process_node * next;

};

#endif