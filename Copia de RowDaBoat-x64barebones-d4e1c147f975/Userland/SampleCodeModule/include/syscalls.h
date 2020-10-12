//syscalls.h
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
extern void syscall_get_pid();
extern void syscall_create_process(); //hay q ver q carajo recibe
extern void syscall_kill(uint64_t pid);
extern void syscall_change_state(uint64_t pid);
extern void syscall_ps();
extern void syscall_set_priority(uint64_t pid, uint64_t new_priority);

