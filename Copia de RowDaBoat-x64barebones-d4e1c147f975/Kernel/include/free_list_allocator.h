#include <stdint.h>

void free_list_init(void * p, uint64_t total_mem);
void * free_list_malloc(uint64_t size_bytes);
void free_list_free(void * ptr);  
void free_list_check_mem_state(uint64_t * state);
