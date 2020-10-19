#include <stdint.h>
#include <lib.h>
#include <scheduler.h>
#include <buddy_allocator.h>
#include <time.h> 
#include <interrupts.h>
#include <screen_driver.h>

#define NULL (void *) 0
#define LOWEST_PRIO 1
#define HIGHEST_PRIO 10

#define QUANTUM 100

static pcb * current; 
static pcb * idle_process;
static pcb * first, *last;
static int counter = 0;

uint64_t scheduler(uint64_t stack_pointer){
    timer_handler();
    if(first == NULL){
        init_scheduler();
    }
    current->sp = stack_pointer;
    counter++;
    if(counter >= current->priority || current == idle_process){
        if(current->state == 2){
            current->state = 1;
        }
        pcb * aux = get_next();
        current = aux;
        current->state = EXECUTING;
        counter = 0;
        drawString("Running: ");
        drawString(current->name);
        drawString("\n");
        return current->sp;
    }
    return current->sp;
}

pcb * get_next(){
    if(first->next == NULL){
        return idle_process; 
    }

    //printListt();
    pcb * aux = current->next;
    if(aux == NULL){
        aux = first->next;
    }
    // IDLE -> SHELL -> NULL
    while(aux->state != READY && aux->pid != current->pid){
        aux = aux->next;
        if(aux == NULL){
            if(first == current){
                aux = first;
            }
            else{
                aux = first->next;
            }
        }
    }
    if(aux->pid == current->pid){ //dimos toda la vuelta
        if(aux->state != READY){
            return idle_process; //no encontre ninguno excepto el que corri antes, asique lo mando otra vez
        }
    }
    return aux;
}

void idle(){
    while (1){
        _hlt();
    }
}

void init_scheduler(){
    //generate idle ... 
    idle_process = generate_process("IDLE", &idle, LOWEST_PRIO, BACK);
    drawString("Generating scheduler...\n");
    current = idle_process;
    first = idle_process;
    last = first;
}

void force_new_selection(){
   
    counter = current->priority + 1;
    call_scheduler();
    // scheduler(current->sp);
}

state get_pcb_state(uint64_t pid){
    pcb * aux = get_pcb(pid);
    if(aux == NULL){
        return NOT_FOUND; 
    }
    return aux->state;
}

//cambia a usar dos funciones para llamar una desde syscalss solo con el pid
uint64_t change_state(uint64_t pid){
    state aux = get_pcb_state(pid);
    if(aux == READY){
        set_state(pid, BLOCKED);
        return 1;
    }
    if(aux == BLOCKED){
        set_state(pid, READY);
        return 1;
    }
    if(aux == EXECUTING){
        set_state(pid, BLOCKED);
        call_scheduler();
        return 1;
    }
    return 0; //NOT_FOUND
}

void set_state(uint64_t pid, state state){
    pcb * aux = get_pcb(pid);
    aux->state = state;
}

uint64_t set_priority(uint64_t pid, uint64_t new_priority){
    pcb * aux = get_pcb(pid);
    if(aux == NULL){
        return 0;
    }
    aux->priority = new_priority;
    return 1;
}   

uint64_t get_pid(){
    if(current == NULL){
        return NULL;
    }
    return current->pid;
}

uint64_t get_ppid(){
    if(current == NULL){
        return 0;
    }
    return current->ppid;
}

pcb * get_prev(){
    pcb * aux = first;
    while(aux->next->pid != current->pid){
        aux = aux->next;
    }
    return aux;
}

//retorna 0 si no pudo y 1 si tuvo exito
uint64_t kill_process(uint64_t pid){
    if(current->pid == pid){
        pcb * current_prev = get_prev();
        current_prev->next = current->next;
        buddy_free(current);
        if(current == last){
            last = current_prev;
            current = first;
        } else{
            current = current->next;
        }
        call_scheduler();
        return 1;
    }

    pcb * aux = first;
    pcb * aux_prev = aux;

    while(aux != NULL && aux->pid != pid){
        aux_prev = aux;
        aux = aux->next;
    }
    if(aux == NULL){
        return 0; //no lo encontré. 
    }
    if(aux == first){
        first = first->next;
    }
    else if(aux == last){
        last = aux_prev;
        aux_prev->next = NULL;
    }
    else{
        aux_prev->next = aux->next;
    }
    buddy_free(aux);
    return 1;
    
}

// //sacarla y cambiarla por block
// void toggle_block(uint64_t pid){
//     pcb * aux = get_pcb(pid);

//     if(aux == NULL){ //mal pasado el pid
//         return;
//     }

//     if(aux->state == BLOCKED){
//         aux->state = READY;
//         return;
//     }

//     if(aux->state == EXECUTING){
//         aux->state = BLOCKED;
//         call_scheduler();
//         //busco nuevo proceso a ejecutar
//     }
//     aux->state = BLOCKED;
// }

uint64_t block(uint64_t pid){
    pcb * aux = get_pcb(pid);

    if(aux == NULL){ //mal pasado el pid
        return 0;
    }

    if(aux->state == EXECUTING){
        set_state(pid, BLOCKED);
        call_scheduler();
        return 1;
    }
    if(aux->state == READY || aux->state == BLOCKED){
        set_state(pid, BLOCKED);
        return 1;
    }
    return 0; 
}

uint64_t unblock(uint64_t pid){
    pcb * aux = get_pcb(pid);

    if(aux == NULL){ //mal pasado el pid
        return 0;
    }

    if(aux->state == BLOCKED || aux->state == READY){
        set_state(pid, READY);
        return 1;
    }
    return 0;
}


pcb * get_pcb(uint64_t pid){
    if(first != NULL){
        pcb * aux = first; 
        
        while(aux->next != NULL && aux->pid != pid){
            aux = aux->next;
        }
        if(aux->pid == pid ){
            return aux;
        }
    }
    return NULL;
}

void add_process(pcb * process){
    last->next = process;
    last = last->next;
}

void print_all(){
    if(first == NULL){
        drawString("There is no process in the scheduler at the moment\n");
        return;
    }
    pcb * pcb_aux = first;
    drawString("Name      Pid     PPid     Priority    State        BP          SP        Type\n");
    for(;pcb_aux != NULL; pcb_aux = pcb_aux->next){
        drawString(pcb_aux->name);
        drawString("       ");
        drawNumber(pcb_aux->pid, 0xFFFFFF, 0x000000);
        drawString("       ");
        drawNumber(pcb_aux->ppid, 0xFFFFFF, 0x000000);
        drawString("         ");
        drawNumber(pcb_aux->priority, 0xFFFFFF, 0x000000);
        drawString("       ");
        if(pcb_aux->state == READY){
            drawString("Ready");
        }
        else if(pcb_aux->state == BLOCKED){
            drawString("Blocked");
        }
        else if(pcb_aux->state == EXECUTING){
            drawString("Executing");
        }
        drawString("     ");
        drawNumber(pcb_aux->bp, 0xFFFFFF, 0x000000);
        drawString("     ");
        drawNumber(pcb_aux->sp, 0xFFFFFF, 0x000000);
        drawString("       ");
        if(pcb_aux->type == FORE){
            drawString("Foreground");
        }
        else if(pcb_aux->type == BACK){
            drawString("Background");
        }
        drawString("\n");
    }
}