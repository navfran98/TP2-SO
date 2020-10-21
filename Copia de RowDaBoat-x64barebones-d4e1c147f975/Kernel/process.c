#include <stdint.h>
#include <process.h>
#include <lib.h>
#include <memoryManager.h>


#define NULL (void*) 0
#define SIZE_OF_STACK 1000 //FIJARSE DESPUES

uint64_t process_count = 0;
//ACORDARNOS REVISAR COMENTARIOS DEL INIT_STACK
pcb * generate_process(char * name, void * rip, uint64_t priority,  process_type type){

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
    to_ret->bp = init_stack(start_address, rip);
    to_ret->sp = to_ret->bp;
    to_ret->stack = stack_ptr;
    to_ret->state = 1;
    to_ret->type = type;
    to_ret->priority = priority;
    to_ret->pid = process_count;
    to_ret->ppid = get_pid(); 
    to_ret->next = NULL;

    process_count++;

    return to_ret;
}

uint64_t create_process(char * name, void * rip, uint64_t priority,  process_type type){
    pcb * pcb = generate_process(name, rip, priority, type);
    if(pcb == NULL){
        return -1;
    }
    add_process(pcb);
    return (process_count - 1);
}

void free_process(pcb * p){
    my_free(p->stack);//... hago free de todo lo que hice malloc.
    my_free(p);
}
