#include <stdint.h>

void * my_malloc(uint64_t size);
void my_free(void * ptr);
void my_check_mem_state();