#include "scheduler.h"
#include <drivers.h>
#include "process.h"
#include "stack.h"
#include "interruptions.h"
#include "ioBlocked.h"
#include <naiveConsole.h>
#include "Queue.h"

#define FLIPED 5

static struct scheduler * scheduler;

typedef int (*EntryPoint)();

// static struct process * mainProcess;
// static struct process * processes[10];

static int pid = 0;

static Queue q;

void initScheduler() {
	scheduler = (struct scheduler *)malloc(sizeof(struct scheduler));
	queueInit(&q, sizeof(struct process *));
}

int getCurrentPid() {
	return scheduler->current->process->pid;
}

void * getCurrentSP() {
	return scheduler->current->process->userStack;
}


void * switchUserToKernel(void * esp) {

	if (scheduler->current->process->flipped != FLIPED) {
		scheduler->current->process->userStack = esp;
		return scheduler->current->process->kernelStack;
	} else {
		scheduler->current->process->kernelStack = esp;
		return scheduler->current->process->testStack;
	}
}


void * switchKernelToUser(void * esp) {
	if (scheduler->current->process->flipped != FLIPED) {
		scheduler->current->process->kernelStack = esp;
		return scheduler->current->process->userStack;
	} else {
		scheduler->current->process->testStack = esp;
		return scheduler->current->process->kernelStack;
	}
}

void * switchStack(void * from_rsp, void * to_rsp) {

}

void * swap(void * from_rsp, void * to_rsp) {

}

void unflip() {
	scheduler->current->process->flipped = 0;
}

void next() {
	scheduler->current = scheduler->current->next;
	while (scheduler->current->process->state != RUNNING){
		progress();
		scheduler->current = scheduler->current->next;
	}
}

char *vid = (char *) 0xB8000;
int proC = 0;
void progress() {
	char pro[] = {'\\', '|', '/', '-'};
	vid[158] = pro[proC%4];
	proC++;
}

void schedule() {

	if (isEmpty()) {
		next();
		

	} else {

		struct process * process1 = pop();

		addProcess(process1); //Lo pongo como next.
		next(); //Paso al next, lo pongo como current.
		callProcess(process1);
	}
}

// El problema es que bloqueo a un proceso y despues levanto un proceso con el call.
// Eso rompe bastante todo.
void startProcess(struct process * process) {

	process->state = RUNNING;

	addProcess(process); //Lo pongo como next.

	next(); //Paso al next, lo pongo como current.

	enableTickInter(); // Lo hago aca, porque es posible que el primer tick, entre antes que hayan procesos en el scheduler.

	callProcess(process);
}

void queueProcess(struct process * process) {

	push(process);
}

void addProcess(struct process * process) {


	struct process_node * newNode; //Creo un node nuevo
	newNode = (struct process_node *)malloc(sizeof(struct process_node)); //Lo aloco


	newNode->process = process; //Le asigno el proceso
	newNode->process->pid = pid; //Le asigno el pid


	if (pid == 0) { //Es el primero?
		scheduler->current = newNode;
		scheduler->current->next = newNode; //Lo pongo como current y el que le sigue a current
	} else {
		newNode->next = scheduler->current->next;
		scheduler->current->next = newNode; //Lo pongo como el siguiente al current y cambio las referencias del siguiente previo.
	}
	pid++; //Aumento el pid
}

void unblock(int code) {
	// if (!ioIsEmpty()){
		struct process * ready;// = ioPop();
	// 	ncPrintHex(ready->pid); //<=--=-=-=-=-=-=-=-=-=-=<<<
	// 	ready->state = RUNNING;
	// 	// ready->flipped = 0;
	// }else{
	// 	ncPrint(":(!!!!!!!!!!!!!!!!");
	// }
	if(getQueueSize(&q) > 0)
    {
        dequeue(&q, &ready);
        ready->state = RUNNING;
    }
}

void flip() {
	scheduler->current->process->flipped = FLIPED;
}

void blockCurrent(int code) {

	scheduler->current->process->state = KEYBOARD_BLOCK;


	// ioPush(scheduler->current->process);
	enqueue(&q, &(scheduler->current->process));

	// while(1);
	
	// enableTickInter();
	// endInter();
	// pushAQ();
	// schedule();

	ncPrint("Salio");

	int20();
	
	// irq0Handler();
}


