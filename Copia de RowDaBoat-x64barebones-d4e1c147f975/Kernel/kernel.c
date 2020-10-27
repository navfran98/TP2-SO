// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdint.h>
#include <lib.h>   // ya que necesita la funcion cpuvendor
#include <naiveConsole.h>
#include <moduleLoader.h>
#include <memoryManager.h>
#include <idtLoader.h>
#include <scheduler.h>
#include <screen_driver.h>   // creo que es solo para probar cosas, despues seguro se puede sacar
#include <video_driver.h>   // para incluir la funcion init_video_driver
#include <time.h>   // seguro despues lo puedo sacar
#include <kernel.h>
#include <interrupts.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;
static void * base_mem = (void *) 0x800000;  
static uint64_t total_mem = 1024 * 1024 * 10;

typedef int (*EntryPoint)();


void clearBSS(void * bssAddress, uint64_t bssSize) {
	memset(bssAddress, 0, bssSize);
}

void * getStackBase() {
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary() {

	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	}; 

	loadModules(&endOfKernelBinary, moduleAddresses);

	clearBSS(&bss, &endOfKernel - &bss);

	init_mm(base_mem, total_mem);
	init_VM_Driver();
	init_scheduler();
	
	return getStackBase();
}

int main() {
	//crear la shell
	create_process("SHELL", sampleCodeModuleAddress, 0, FORE);
	load_idt();

	_hlt();

	drawString("If this is shown, then something is wrong! \n");
	
	return 0;
}


