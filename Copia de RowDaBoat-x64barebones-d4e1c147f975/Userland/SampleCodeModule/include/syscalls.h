#include <stdint.h>

//Memory Manager 
extern void * syscall_malloc(unsigned size);
extern void syscall_free(void * ptr);
extern void syscall_check_mem_state();

//R and W 
extern void syscall_write(char* toWrite, int length);
extern void syscall_read(int file_descriptor, char* toRead,int length);

//Scheduler
extern void syscall_ps();
extern void syscall_halt();
extern uint64_t syscall_get_pid();
extern uint64_t syscall_get_pipe_id();
extern void syscall_force_new_selection();
extern uint64_t syscall_kill(uint64_t pid);
extern uint64_t syscall_block(uint64_t pid);
extern uint64_t syscall_unblock(uint64_t pid);
extern uint64_t syscall_change_state(uint64_t pid);
extern uint64_t syscall_set_priority(uint64_t pid, uint64_t new_priority);
extern uint64_t syscall_create_process(char * name, void * ptr, uint64_t pipe_id, uint64_t ground);