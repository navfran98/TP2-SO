#include <stdint.h>

void * my_malloc(uint8_t size_bytes);
void my_free(void * ptr);  
void check_mem_state(unsigned * state);
