#include <stdint.h>

typedef enum state {BLOCKED,READY,EXECUTING,NOT_FOUND} state;
typedef enum process_type {FORE, BACK} process_type;

typedef struct process_data{
    char * name;
    uint64_t pid;
    uint64_t ppid;
    uint64_t priority;
    uint64_t sp;
    uint64_t bp;
    void * stack;

    enum state state;
    enum process_type type;

    pcb * next;

} pcb; 


pcb * generate_process(char * name, uint64_t priority, 
uint64_t pid, uint64_t ppid, uint64_t stack_pointer, 
uint64_t base_pointer, enum state state, process_type type);