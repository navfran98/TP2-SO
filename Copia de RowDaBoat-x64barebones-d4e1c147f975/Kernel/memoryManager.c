// #include <stdint.h>

// #ifdef BUDDY
//     void * my_malloc(uint64_t size){
//         return buddy_malloc(size);
//     }

//     void my_free(void * ptr){
//         return buddy_free(ptr);
//     }

//     void my_check_mem_state(){
//         return buddy_check_mem_state();
//     }
// #else
// void * my_malloc(uint64_t size){
//     return free_list_malloc(size);
// }

// void my_free(void * ptr){
//     return free_list_free(ptr);
// }

// void my_check_mem_state(){
//     return free_list_check_mem_state();
// }
//     }

// #endif

