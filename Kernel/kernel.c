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


extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

char *videoDeb = (char *) 0xB8000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;

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
int sysCallDispatcher(int function, char* segundo, int tercero, int cuarto) {

	switch (function) {
	case SYSCALL_WRITE: {
		switch ( cuarto ) {
		case DESCRIPTOR_CLI: {
			write(segundo, tercero);
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
			int res = 0;
			if (res == 0) {
				blockCurrent(KEYBOARD_BLOCK);
				// ncPrint("Test");
				unflip();
			}
			// char sux[] = "test";
			// for (int i = 0; i < 5; i++) {
			// 	segundo[i] = sux[i];
			// }
			return read(segundo);
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
	case SYSCALL_MALLOC: {
		return malloc(tercero);
		break;
	}
	case SYSCALL_CALLOC: {
		return calloc(tercero);
		break;
	}
	case SYSCALL_FREE: {
		free(segundo);
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

void * toStackAddress(void * page) {
	return page + 0x1000 - 0x10;
}

int main() {

	// Kernel INIT
	dma_start();

	// Kernel Operations
	setKeyboardCallback(miCallbackDeTeclado);

	// Net Init
	dma_start();
	net_start();


	initScheduler();

	init_interruptions();



	struct process * process1;
	process1 = malloc(sizeof(struct process));
	process1->entryPoint = sampleCodeModuleAddress;
	process1->userStack = toStackAddress(malloc(0x1000));
	process1->kernelStack = toStackAddress(malloc(0x1000));
	queueProcess(process1);


	struct process * process3;
	process3 = malloc(sizeof(struct process));
	process3->entryPoint = sampleCodeModuleAddress;
	process3->userStack = toStackAddress(malloc(0x1000));
	process3->kernelStack = toStackAddress(malloc(0x1000));
	// queueProcess(process3);


	struct process * process4;
	process4 = malloc(sizeof(struct process));
	process4->entryPoint = sampleCodeModuleAddress;
	process4->userStack = toStackAddress(malloc(0x1000));
	process4->kernelStack = toStackAddress(malloc(0x1000));
	// queueProcess(process4);


	struct process * process2;
	process2 = malloc(sizeof(struct process));
	process2->entryPoint = sampleCodeModuleAddress;
	process2->userStack = toStackAddress(malloc(0x1000));
	process2->kernelStack = toStackAddress(malloc(0x1000));

	startProcess(process2);


	while (1);


	return 0;

}




