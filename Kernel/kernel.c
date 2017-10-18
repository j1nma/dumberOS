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
int sysCallDispacher(int function, char* segundo, int tercero, int cuarto){

	switch(function){
		case SYSCALL_WRITE:{
			switch ( cuarto ) {
				case DESCRIPTOR_CLI: {
					write(segundo, tercero);
					break;
				}
				case DESCRIPTOR_NET: {
					int i = 0;
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
		case SYSCALL_READ:{
			switch ( cuarto ) {
				case DESCRIPTOR_CLI: {
					// int ret;
					// while ((ret = read(segundo)) == 0){
					// 	blockMain();
					// }
					// return ret;
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
		case SYSCALL_MALLOC:{
			return malloc(tercero);
			break;
		}
		case SYSCALL_CALLOC:{
			return calloc(tercero);
			break;	
		}
		case SYSCALL_FREE:{
			free(segundo);
			break;	
		}
		case SYSCALL_TIME:{
			getTime(segundo);
			break;
		}
		default:{
			ncPrint("SysCall not found.");
			break;
		}
	}
	return 0;
	
}



void miCallbacldeTeclado(uint8_t c, int function){

	switch(function){
		case RESPONSE_CHARACTER:{
			write(&c, 1); //RESPONSE_CHARACTER es cuando el usuario presiona una tecla imprimible. Llamo a write del driver de video.
			break;
		}
		case RESPONSE_BACKSPACE:{
			backspace();
			break;
		}
		case RESPONSE_ENTER:{
			newLine(); //RESPONSE_ENTER es cuando el usuario presiona "return". Llamo a "newLine" del driver de video.
			break;
		}
		case RESPONSE_ARROWS:{
			break;
		}
	}
}

struct StackFrame {
	//Registers restore context
	// uint64_t gs;
	// uint64_t fs;
	uint64_t r15;
	uint64_t r14;
	uint64_t r13;
	uint64_t r12;
	uint64_t r11;
	uint64_t r10;
	uint64_t r9;
	uint64_t r8;
	uint64_t rsi;
	uint64_t rdi;
	uint64_t rbp;
	uint64_t rdx;
	uint64_t rcx;
	uint64_t rbx;
	uint64_t rax;

	//iretq hook
	uint64_t rip;
	uint64_t cs;
	uint64_t eflags;
	uint64_t rsp;
	uint64_t ss;
	uint64_t base;
};

void * fillStackFrame(void * entryPoint, void * userStack) {
	struct StackFrame *frame = (struct StackFrame *)userStack - 1;
	frame->r15 =	0x003;
	frame->r14 =	0x004;
	frame->r13 =	0x005;
	frame->r12 =	0x006;
	frame->r11 =	0x007;
	frame->r10 =	0x008;
	frame->r9 =		0x009;
	frame->r8 =		0x00A;
	frame->rsi =	0x00B;
	frame->rdi =	0x00C;
	frame->rbp =	0x00D;
	frame->rdx =	0x00E;
	frame->rcx =	0x00F;
	frame->rbx =	0x010;
	frame->rax =	0x011;
	frame->rip =	(uint64_t)entryPoint;
	frame->cs =		0x008;
	frame->eflags = 0x202;
	frame->rsp =	(uint64_t)&(frame->base);
	frame->ss = 	0x000;
	frame->base =	0x000;

	return frame;
}

void * toStackAddress(void * page) {
	return page;
}

int main(){

	// Kernel INIT
	init_interruptions();

	// Kernel Operations
	setKeyboardCallback(miCallbacldeTeclado);	

	// Net Init
	dma_start();
	net_start();


	//Process * process = new Process((void*)sampleCodeModuleAddress);
	static struct process * process;


	
	process->entryPoint = sampleCodeModuleAddress;
	process->userStack = toStackAddress(malloc(0x1000));
	process->kernelStack = toStackAddress(malloc(0x1000));
	process->userStack = fillStackFrame(sampleCodeModuleAddress, process->userStack);
	//process->kernelStack = fillStackFrame(sampleCodeModuleAddress, process->kernelStack);

	addProcess(process);

	setPicMaster(0x04);

	// UserLand Init
	((EntryPoint)sampleCodeModuleAddress)();




	return 0;

}




