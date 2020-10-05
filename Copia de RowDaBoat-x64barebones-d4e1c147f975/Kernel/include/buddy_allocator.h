#include <stdint.h>

void * buddy_malloc(uint64_t size);
void buddy_free(void * ptr);
void buddy_check_mem_state(uint64_t * state);
void generate_buddy_tree(void * base_mem, uint64_t total_mem);