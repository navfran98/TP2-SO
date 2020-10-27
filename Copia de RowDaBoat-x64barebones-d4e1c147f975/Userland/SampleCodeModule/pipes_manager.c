#include <stdint.h>
#include <stdio.h>
#include <syscalls.h>
#include <pipes_manager.h>
#include <semaphores_manager.h>

#define BUFFER_SIZE 1024
#define NULL ((void *) 0)
extern char* num_to_string(int num);
static pipe * first = NULL;
static pipe * last = NULL;
uint16_t pipe_count = 1;


void unblock_pipe_partner(uint64_t pipe_id){
    if(pipe_id == 0){
        return;
    }
    pipe * p = get_pipe(pipe_id);
    syscall_unblock(p->pid1);
    syscall_unblock(p->pid2);
}

pipe * get_pipe(uint64_t id){
    pipe * aux = first;
    while(aux != NULL && aux->pipe_id != id){
        aux = aux->next;
    }
    return aux;
}

pipe * create_pipe(){
    pipe * p = syscall_malloc(sizeof(pipe));
    p->pipe_id = pipe_count++;
    p->pid1 = 0;
    p->pid2 = 0;
    p->nread = 0;
    p->nwrite = 0;
    p->lock = 0;
    p->next = NULL;
    if(first == NULL){
        first = last = p;
    }else{
        last->next = p;
        last = last->next;
    }
    return p;
}

int kill_pipe(uint64_t id){
    pipe * aux = first;
    pipe * aux_prev = aux;
    while(aux != NULL){
        if(aux->pipe_id == id){
            if(aux == first){
                first = aux->next;
            }
            if(aux == last){
                if(aux_prev != aux){
                    last = aux_prev;
                }
                last = NULL;
            }
            aux_prev->next = aux->next;
            syscall_free(aux);
            return 1;
        }
        aux_prev = aux;
        aux = aux->next;
    }
    return 0;
}

//MODIFICAR PARA Q RECIBA SOLO EL PIPE_ID Y EL STRING
int pipe_write(uint64_t pipe_id, char * addr, int n){
    pipe * p = get_pipe(pipe_id);
    int i;
    for(i=0;i<n;i++){
        while(1){
            acquire(&p->lock);
            if(p->nwrite < p->nread + BUFFER_SIZE)
                break;
            release(&p->lock);
        }
        p->buffer[p->nwrite++] = addr[i];
        release(&p->lock);
    }
    acquire(&p->lock);
    p->buffer[p->nwrite++] = '\0';
    release(&p->lock);
    return n; 
}

//MODIFICAR PARA Q RECIBA SOLO EL PIPE_ID Y EL BUFFER QUE VA A LLENAR
int pipe_read(uint64_t pipe_id, char* addr){
    pipe * p = get_pipe(pipe_id);
    int i=0;
    // for(i = 0; i < n; i++){
    while(i<BUFFER_SIZE){
        while(1){
            acquire(&p->lock);
            if(p->nread < p->nwrite)
                break;
            release(&p->lock);
        }
        if(p->buffer[p->nread] == '\0'){
            addr[i++] = p->buffer[p->nread++];
            release(&p->lock);
            break;
        }
        addr[i++] = p->buffer[p->nread++];
        release(&p->lock);
    }
    return i;    
}

void print_all_pipes(){
    print("ESTOY EN PRINT PIPES\n");
    if(first == NULL){
        print("There isn't a single pipe\n");
        return;
    }
    print("Pipe_id      Number of bytes written      Number of bytes read\n");
    for(pipe * p = first; p != NULL; p = p->next){
        print(num_to_string(p->pipe_id));
        print("                    ");
        print(num_to_string(p->nwrite));
        print("                    ");
        print(num_to_string(p->nread));
        print("                    ");
        print("\n");
    }
}