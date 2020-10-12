#include <stdint.h>

typedef enum {BLOCKED=0,READY,EXECUTING,NOT_FOUND} state;
typedef enum {FORE=0, BACK} process_type;

typedef struct process_data{
    char * name;
    uint64_t pid;
    uint64_t ppid;
    uint64_t priority;
    uint64_t sp;
    uint64_t bp;
    void * stack;

    state state;
    process_type type;

    struct process_data * next;

}pcb; 


pcb * generate_process(char * name, void * rip, uint64_t priority,  process_type type);
uint64_t create_process(char * name, void * rip, uint64_t priority,  process_type type);
