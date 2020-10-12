#include <stdint.h>
#include <process.h>
#include <lib.h>

#define NULL (void*) 0
#define SIZE_OF_STACK 1000 //FIJARSE DESPUES

uint64_t process_count = 0;
//ACORDARNOS REVISAR COMENTARIOS DEL INIT_STACK
pcb * generate_process(char * name, void * rip, uint64_t priority,  process_type type){

    void * stack_ptr = buddy_malloc(SIZE_OF_STACK);
    char * start_address = (char * ) stack_ptr + SIZE_OF_STACK; //direccion mas baja del stack
    

    pcb * to_ret = buddy_malloc(sizeof(pcb));
    to_ret->name = name;
    to_ret->bp = init_stack(stack_ptr, rip);
    to_ret->sp = to_ret->bp;
    to_ret->stack = stack_ptr;
    to_ret->state = READY;
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
    add_process(pcb);
}

void free_process(pcb * p){
    //buddy_free(p->sp);//... hago free de todo lo que hice malloc.
    buddy_free(p);
}
