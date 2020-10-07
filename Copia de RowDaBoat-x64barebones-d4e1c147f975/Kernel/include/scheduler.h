#include <process.h>

enum state {BLOCKED,READY,EXECUTING};
enum process_type {FORE, BACK};

typedef struct scheduler_list{
    pcb * first;
    pcb * last;
    //podemos agregar las cosas que necesitamos a esto.
} schlist; 


static pcb * first;

// typedef struct list_node{
//     process process;
//     struct list_node * next;
// } node;

uint64_t set_priority(uint64_t pid, uint64_t new_priority);

uint64_t get_pid();

uint64_t get_ppid();

uint64_t kill_process(uint64_t pid);

void print_all();

void toggle_block(uint64_t pid);

//para usar dentro de las demas funciones
pcb * get_pcb(uint64_t pid);

void add_process(pcb * process);