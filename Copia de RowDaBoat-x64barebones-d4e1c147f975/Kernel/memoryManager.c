// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdint.h>
#include <free_list_allocator.h>
#include <buddy_allocator.h>
//#include "memoryManager.h"
//#include <memMode.h>
//#define BUDDY 3

#ifdef BUDDY
    void init_mm(void * base_mem, uint64_t total_mem){
        generate_buddy_tree(base_mem, total_mem); 
    }

    void * my_malloc(uint64_t size){
        return buddy_malloc(size);
    }

    void my_free(void * ptr){
        return buddy_free(ptr);
    }

    void my_check_mem_state(){
        return buddy_check_mem_state();
    }
    
#else
    void init_mm(void * base_mem, uint64_t total_mem){
    	free_list_init(base_mem, total_mem); 
    }

    void * my_malloc(uint64_t size){
        return free_list_malloc(size);
    }

    void my_free(void * ptr){
        return free_list_free(ptr);
    }

    void my_check_mem_state(){
        return free_list_check_mem_state();
    }

#endif