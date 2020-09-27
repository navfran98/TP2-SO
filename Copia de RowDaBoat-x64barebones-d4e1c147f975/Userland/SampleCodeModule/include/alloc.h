//alloc.h

extern void * syscall_malloc(unsigned size);
extern void syscall_free(void * ptr);
extern void syscall_check_mem_state(unsigned * vec);
