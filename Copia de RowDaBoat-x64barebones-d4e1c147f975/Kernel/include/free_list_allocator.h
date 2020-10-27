#include <stdint.h>

#define NULL (void *) 0

typedef struct mem_slot{
	struct mem_slot * next;
	struct mem_slot * prev;
	void * location;
	uint64_t size;
} slot;

typedef struct header{
	slot * list;
	slot * used;
	uint64_t used_mem;
	uint64_t free_mem;
	uint64_t total_mem;
} list_header;

void free_list_init(void * p, uint64_t total_mem);
void * free_list_malloc(uint64_t size_bytes);
void free_list_free(void * ptr);  
void free_list_check_mem_state();
