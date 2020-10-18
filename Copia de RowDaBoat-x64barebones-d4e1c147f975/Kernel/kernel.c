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
#include <buddy_allocator.h>

#include <screen_driver.h>   // creo que es solo para probar cosas, despues seguro se puede sacar
#include <video_driver.h>   // para incluir la funcion init_video_driver
#include <time.h>   // seguro despues lo puedo sacar
#include <kernel.h>
#include <process.h>
#include <interrupts.h>
// #include <scheduler.h>


extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

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

	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);

	clearBSS(&bss, &endOfKernel - &bss);

	generate_buddy_tree(base_mem, total_mem);
	//free_list_init(base_mem, total_mem); 
	init_VM_Driver();
	init_scheduler();
	
	return getStackBase();
}

void joaco(){
	while(1){
		_hlt();
	}
}

int main() {
	//crear la shell
	create_process("SHELL", sampleCodeModuleAddress, 5, FORE);
	create_process("JOACO", &joaco, 3, BACK);
	load_idt();

	_hlt();

	drawString("If this is shown, then something is wrong! \n");
	
	return 0;
}


	// uint8_t first_process = create_process((uint64_t)sampleCodeModuleAddress, 1, 0 , 0);
	// if(first_process == 1) {
	// 	drawString("SHELL WAS NOT CREATED --> ABORT");
	// 	haltcpu();
	// }

	// // Arqui function provided by the proffesors. Loads the IDT and fill it with int0, int20, int21 and int80
	// load_idt();

	// // halt the cpu until the TT interruption arrives, which will choose the Shell as the running process
	// _hlt();
	
	// drawString("If this is shown, then something is wrong! \n");
	// return 0;


