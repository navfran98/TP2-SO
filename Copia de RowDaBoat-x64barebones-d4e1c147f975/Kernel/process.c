#include <stdint.h>
#include <process.h>
#include <scheduler.h>
#define NULL (void*) 0

pcb * generate_process(char * name, uint64_t priority, 
uint64_t pid, uint64_t ppid, uint64_t stack_pointer, 
uint64_t base_pointer, process_type type){
    pcb * to_ret = buddy_malloc(sizeof(pcb));
    to_ret->name = name;
    to_ret->bp = base_pointer;
    to_ret->sp = stack_pointer;
    to_ret->state = READY;
    to_ret->type = type;
    to_ret->priority = priority;
    to_ret->pid = pid;
    to_ret->ppid = ppid;
    to_ret->next = NULL;
    return to_ret;
}

uint64_t create_process_and_add_to_scheduler(){
    // pcb * new_process = generate_process(...);

    // add_process(new_process);
}