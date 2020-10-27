#include <stdint.h>
#include <lib.h>
#include <scheduler.h>
#include <memoryManager.h>


#define NULL (void*) 0
#define SIZE_OF_STACK 1000

uint64_t process_count = 0;
pcb * generate_process(char * name, void * rip, uint64_t pipe_id,  process_type type){

    void * stack_ptr = my_malloc(SIZE_OF_STACK);
    if(stack_ptr == NULL){
        return NULL;
    }
    
    char * start_address = (char * ) stack_ptr + SIZE_OF_STACK; //direccion mas baja del stack
    
    pcb * to_ret = my_malloc(sizeof(pcb));
    if(to_ret == NULL){
        return NULL;
    }
    to_ret->name = name;
    to_ret->bp = init_stack(start_address, (uint64_t) rip);
    to_ret->sp = to_ret->bp;
    to_ret->stack = stack_ptr;
    to_ret->state = 1;
    to_ret->type = type;
    if(strcmp(name, "SHELL") == 0){
        to_ret->priority = 5;
    }else{
        to_ret->priority = 1;
    }
    to_ret->pid = process_count;
    to_ret->ppid = get_pid(); 
    to_ret->pipe_id = pipe_id; //id of pipe associated with the process
    to_ret->next = NULL;

    process_count++;

    return to_ret;
}

uint64_t create_process(char * name, void * rip, uint64_t pipe_id,  process_type type){
    pcb * pcb = generate_process(name, rip, pipe_id, type);
    if(pcb == NULL){
        return -1;
    }
    add_process(pcb);
    return (process_count - 1);
}

void free_process(pcb * p){
    my_free(p->stack);
    my_free(p);
}
