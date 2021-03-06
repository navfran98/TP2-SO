#include <stdint.h>

//addresses
static void * const sampleCodeModuleAddress = (void*) 0x400000;
static void * const sampleDataModuleAddress = (void*) 0x500000;
// static void * const shellModuleAddress = (void *) 0x700000;

//functions
void * getStackBase();
void clearBSS(void * bssAddress, uint64_t bssSize);
void * initializeKernelBinary();