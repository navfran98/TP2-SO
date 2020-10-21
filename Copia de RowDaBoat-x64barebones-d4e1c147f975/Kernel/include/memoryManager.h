
void * my_malloc(uint64_t size);
void my_free(void * ptr);
void my_check_mem_state();

void init_mm(void * p, uint64_t total_mem);
