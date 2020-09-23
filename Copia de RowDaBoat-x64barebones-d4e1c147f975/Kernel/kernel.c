#include <stdint.h>
#include <lib.h>   // ya que necesita la funcion cpuvendor

// aunque parece que ya no necesitamos los "ncPrint" ya que estamos en modo video, hay que dejar este
// include ya que la funcion initializeKernelBinary si las usa. No llega a imprimir nada, pero para mi
// no debo borrar esta funcion ya que parece que configura cosas. Tendria que preguntar. 
// Para borrarla hay que: borrarla de aca y tambien del archivo loader.asm
#include <naiveConsole.h>

// lo mismo para con este. Hay que sacar este include solamente si la funcion de initializeKernelBinary
// ya no me sirve (para mi siempre hay que dejarla esta funcion)
#include <moduleLoader.h>

#include <idtLoader.h>     // ya que necesita a load_idt

#include <screen_driver.h>   // creo que es solo para probar cosas, despues seguro se puede sacar
#include <video_driver.h>   // para incluir la funcion init_video_driver
#include <time.h>   // seguro despues lo puedo sacar

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;

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

int main() {
	load_idt();
	init_VM_Driver();
	while(1){             
		((EntryPoint)sampleCodeModuleAddress)();
	}
	return 0;
}
