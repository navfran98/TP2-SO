#include <stdint.h>
#include <semaphores_manager.h>
#include <stdio.h>

static sem * first = NULL;
static sem * last = NULL;
uint16_t sem_count = 0;

//
sem * create_sem(int value){
    sem * new_sem = (sem *) syscall_buddy_malloc(sizeof(sem));
    new_sem->value = value;
    new_sem->sem_id = sem_count++;
    new_sem->lock = 0;
    new_sem->next = NULL;
    list_header * pid_list = syscall_buddy_malloc(sizeof(list_header));
    pid_list->first = pid_list->last = NULL;
    new_sem->blocked_pid_list = pid_list;
    if(first == NULL){
        first = new_sem;
        last = new_sem;
    } else{
        last->next = new_sem;
        last = new_sem;
    }
    return new_sem;
}

//retorna 1 si lo destruye y 0 si ocurre algun error
int kill_sem(uint64_t sem_id){
    //search for the sem and take it out from the list
    sem * aux = first;
    sem * aux_prev = aux;

    while(aux != NULL && aux->sem_id != sem_id){
        aux_prev = aux;
        aux = aux->next;
    }

    if(aux == NULL){
        //not found
        return 0;
    }

    if(aux == aux_prev){ //kill first
        first = aux->next;
    } else if(aux == last){
        last = aux_prev;
        aux_prev->next = NULL;
    } else{
        aux_prev->next = aux->next;
    }

    //unblock all processes and free
    list_node * node = aux->blocked_pid_list->first;
    
    while(node != NULL){
        sem_post(aux, node->pid);
        node = node->next;
    }

    syscall_buddy_free(aux->blocked_pid_list);
    syscall_buddy_free(aux);
    return 1;
}

void release(int * lock){
    _xchg(lock, 0);
}

void acquire(int * lock){
    while(_xchg(lock, 1) != 0);
}

int hasPid(sem * s, uint64_t pid){
    list_node * aux = s->blocked_pid_list->first;
    while(aux != NULL){
        if(aux->pid == pid){
            return 1;
        }
        aux = aux->next;
    }
    return 0;
}

//no sabemos si hay q usar &(s->lock) o s->lock
void sem_wait(sem * s, uint64_t pid){
    print("entre al sem wait\n");
    while(1){
        print("Toy en el semwait!!!!!!!!\n");
        acquire(&(s->lock));
        print("ADQUIRI EL LOCK\n");
        if(s->value > 0){
            s->value--;
            release(&(s->lock));
            break;
        }
        if(hasPid(s, pid) == 0){
            if(syscall_block(pid) == 0){
                print("Error bloqueando proceso en el sem_wait\n");
                return;
            }
            list_node * node = syscall_buddy_malloc(sizeof(list_node));
            node->pid = pid;
            node->next = NULL;
            if(s->blocked_pid_list->first == NULL){
                s->blocked_pid_list->first = s->blocked_pid_list->last = node;
            }else{
                s->blocked_pid_list->last->next = node;
                s->blocked_pid_list->last = node;
            }
        }
        release(&(s->lock));
    }
    print("salgo del sem Wait\n");
}

void sem_post(sem * s, uint64_t pid){
    acquire(&(s->lock));
    s->value++;
    if(s->value == 1){ 
        if(s->blocked_pid_list->first == NULL){
            release(&(s->lock));
            return;
        }
        list_node * aux = s->blocked_pid_list->first;
        if(aux->next == NULL){
            s->blocked_pid_list->first = s->blocked_pid_list->last = NULL;
        }else{
            s->blocked_pid_list->first = aux->next;
        }
        if(syscall_unblock(aux->pid) == 0){
            print("Error al hacer unblock en sem_post\n");
            release(&(s->lock));
            return;
        }
        syscall_buddy_free(aux);
    } 
    release(&(s->lock));
}




void print_all_semaphores(){
    //hacer una funcion como el print_all de scheduler
}

