#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <interruptions.h>
#include <drivers.h>
#include <syscalls.h>
#include <process.h>
#include <scheduler.h>

#include "asynchronousMP.h"
#include "queue.h"
#include "mutualExclusion.h"


extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

char * videoDeb = (char *) 0xB8000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;
static void * const buddyAllocationMemory = (void*)0x600000;

typedef int (*EntryPoint)();


void clearBSS(void * bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
	return (void*)(
	           (uint64_t)&endOfKernel
	           + PageSize * 8				//The size of the stack itself, 32KiB
	           - sizeof(uint64_t)			//Begin at the top of the stack
	       );
}

void * initializeKernelBinary()
{
	char buffer[10];

	ncPrint("[x64BareBones]");
	ncNewline();

	ncPrint("CPU Vendor:");
	ncPrint(cpuVendor(buffer));
	ncNewline();

	ncPrint("[Loading modules]");
	ncNewline();
	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);
	ncPrint("[Done]");
	ncNewline();
	ncNewline();

	ncPrint("[Initializing kernel's binary]");
	ncNewline();

	clearBSS(&bss, &endOfKernel - &bss);

	ncPrint("  text: 0x");
	ncPrintHex((uint64_t)&text);
	ncNewline();
	ncPrint("  rodata: 0x");
	ncPrintHex((uint64_t)&rodata);
	ncNewline();
	ncPrint("  data: 0x");
	ncPrintHex((uint64_t)&data);
	ncNewline();
	ncPrint("  bss: 0x");
	ncPrintHex((uint64_t)&bss);
	ncNewline();

	ncPrint("[Done]");
	ncNewline();
	ncNewline();
	return getStackBase();
}

/**
Dispatch syscalls to the corresponding functions

@param	function Syscall to call
@param	segundo Pointer to string
@param	tercero	Length
@param	cuarto Decryptor to use
*/
void * sysCallDispatcher(int function, char * segundo, int tercero, int cuarto) {

	switch (function) {
	case SYSCALL_WRITE: {
		switch ( cuarto ) {
		case DESCRIPTOR_CLI: {
			// write(segundo, tercero);
			for (int i = 0; i < tercero; i++) {
				lcPrintChar(segundo[i]);
			}
			break;
		}
		case DESCRIPTOR_NET: {
			net_send(segundo);
			break;
		}
		default: {
			ncPrint("SysCall not found.");
			break;
		}
		}
		break;
	}
	case SYSCALL_READ: {
		switch ( cuarto ) {
		case DESCRIPTOR_CLI: {

			blockCurrent(KEYBOARD_BLOCK);

			int test = read(segundo);

			return test;
			break;
		}
		case DESCRIPTOR_NET: {
			return net_read(segundo);
			break;
		}
		default: {
			ncPrint("SysCall not found.");
			break;
		}
		}
		break;
	}
	case SYSCALL_ALLOC: {
		return allocNPages(tercero);
		break;
	}
	case SYSCALL_FREE: {
		freeSpace(segundo);
		break;
	}
	case SYSCALL_TIME: {
		getTime(segundo);
		break;
	}
	case SYSCALL_PID: {
		return getCurrentPid();
		break;
	}
	case SYSCALL_SEND: {
		return asyncSend(segundo, tercero);
		break;
	}
	case SYSCALL_RECEIVE: {
		return asyncReceive();
		break;
	}
	case SYSCALL_UP: {
		unblock(BOLT_BLOCK);
		return releaseBolt(getBolt());
		break;
	}
	case SYSCALL_DOWN: {
		return acquireBolt(getBolt());
		break;
	}
	case SYSCALL_PROCESS: {
		struct process * newProcess = createNewProcess((void *)segundo, getCurrentPid());
		queueProcess(newProcess);
		blockCurrent(CREATE_PROCESS_BLOCK);
		return newProcess->pid;
		break;
	}
	case SYSCALL_PROCESS_KILL: {
		return killProcess(tercero);
		break;
	}
	case SYSCALL_PROCESS_LIST: {
		listProcesses();
		break;
	}
	default: {
		ncPrint("SysCall not found.");
		break;
	}
	}
	return 0;

}



void miCallbackDeTeclado(uint8_t c, int function) {
	switch (function) {
	case RESPONSE_CHARACTER: {
		write(&c, 1); //RESPONSE_CHARACTER es cuando el usuario presiona una tecla imprimible. Llamo a write del driver de video.
		break;
	}
	case RESPONSE_BACKSPACE: {
		backspace();
		break;
	}
	case RESPONSE_ENTER: {
		unblock(KEYBOARD_BLOCK);
		newLine(); //RESPONSE_ENTER es cuando el usuario presiona "return". Llamo a "newLine" del driver de video.
		break;
	}
	case RESPONSE_ARROWS: {
		break;
	}
	}
}


int main() {

	setUpHeapOrganizer(buddyAllocationMemory);
	initMemory();

	// Kernel INIT
	dma_start();

	// Kernel Operations
	setKeyboardCallback(miCallbackDeTeclado);

	// Net Init
	dma_start();

	net_start();

	initScheduler(getRSP());

	initMutex();

	struct process * process0 = createNewProcess(sampleCodeModuleAddress, 0);

	init_interruptions();

	startProcess(process0);

	deleteMutex();

	return 0;

}