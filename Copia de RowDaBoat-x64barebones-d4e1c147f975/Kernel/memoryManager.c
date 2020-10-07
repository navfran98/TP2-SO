#include <stdint.h>

void * my_malloc(uint64_t size){
    // return free_list_malloc(size);
    return buddy_malloc(size);
}

void my_free(void * ptr){
    // return free_list_free(ptr);
    return buddy_free(ptr);
}

void my_check_mem_state(){
    // return free_list_check_mem_state();
    return buddy_check_mem_state();
}

