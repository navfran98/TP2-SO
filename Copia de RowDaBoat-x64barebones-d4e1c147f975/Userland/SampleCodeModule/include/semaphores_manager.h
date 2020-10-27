#include <stdint.h>

#define NULL ((void *) 0)

typedef struct list_node{
    uint64_t pid;
    struct list_node * next;
} list_node;

typedef struct list_header{
    list_node * first;
    list_node * last;
} list_header;

typedef struct semaphore {
    uint16_t sem_id;
    uint8_t value;
    int lock; 
    list_header * blocked_pid_list; // listado de todos los procesos bloqueados por este sem
    struct semaphore * next;
} sem;

extern uint64_t _xchg(int * lock, int n);
//
void release(int * lock);
void acquire(int * lock);
//
sem * create_sem(int value);
int kill_sem(uint64_t sem_id);
void sem_wait(sem * s, uint64_t pid);
void sem_post(sem * s, uint64_t pid);
void print_all_semaphores();
//