#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <interruptions.h>
#include <drivers.h>
#include <syscalls.h>


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

int main(){

	// Kernel INIT
	init_interruptions();

	// Kernel Operations
	setKeyboardCallback(miCallbacldeTeclado);	

	// Net Init
	dma_start();
	net_start();

	// UserLand Init
	((EntryPoint)sampleCodeModuleAddress)();

	return 0;

}

