//alloc.h

extern void * syscall_malloc(unsigned size);
extern void syscall_free(void * ptr);
extern void syscall_check_mem_state(uint64_t * vec);
extern void * syscall_buddy_malloc(unsigned size);
extern void syscall_buddy_free(void * ptr);
extern void syscall_buddy_check_mem_state(uint64_t * vec);
