#include <process.h>
#include <stdint.h>

typedef struct scheduler_list{
    pcb * first;
    pcb * last;
    //podemos agregar las cosas que necesitamos a esto.
} schlist; 

static pcb * first;

pcb * get_next(); 

void idle();

void init_scheduler();

void force_new_selection();

state get_pcb_state(uint64_t pid);

uint64_t change_state(uint64_t pid);

void set_state(uint64_t pid, state state);

uint64_t set_priority(uint64_t pid, uint64_t new_priority);

uint64_t get_pid();

uint64_t get_ppid();

uint64_t kill_process(uint64_t pid);

void print_all();

void toggle_block(uint64_t pid);

pcb * get_pcb(uint64_t pid);

void add_process(pcb * process);