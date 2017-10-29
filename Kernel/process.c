#include "process.h"
#include <drivers.h>

void callProcess(struct process *process) {
	endInter();
	switchStackAndJump((process->userStack), (process->entryPoint));
}