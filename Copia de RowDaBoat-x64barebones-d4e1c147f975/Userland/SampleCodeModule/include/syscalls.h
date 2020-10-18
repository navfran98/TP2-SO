#include <stdint.h>
// #define ID_READ 1
// #define ID_WRITE 2

//Alloc
extern void * syscall_malloc(unsigned size);
extern void syscall_free(void * ptr);
extern void syscall_check_mem_state(uint64_t * vec);
extern void * syscall_buddy_malloc(unsigned size);
extern void syscall_buddy_free(void * ptr);
extern void syscall_buddy_check_mem_state(uint64_t * vec);

//R and W
extern void syscall_write(char* toWrite, int length);
extern void syscall_read(int file_descriptor, char* toRead,int length);

//Scheduler
// extern void syscall_create_process(int ID, char * name, char * s , void * ptr, uint64_t priority, char * ground); //hay q ver q carajo recibe
//hasta ahora son todas voids pero hay q arreglar eso
extern void syscall_ps();
extern void syscall_halt();
extern uint64_t syscall_get_pid();
extern void syscall_force_new_selection();
extern uint64_t syscall_kill(uint64_t pid);
extern uint64_t syscall_block(uint64_t pid);
extern uint64_t syscall_unblock(uint64_t pid);
extern uint64_t syscall_change_state(uint64_t pid);
extern void syscall_set_priority(uint64_t pid, uint64_t new_priority);
extern uint64_t syscall_create_process(char * name, void * ptr, uint64_t priority, uint64_t ground);