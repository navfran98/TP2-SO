#include <stdint.h>

#define BUFFER_SIZE 1024
#define NULL ((void *) 0)

typedef struct pipe {
    uint64_t pipe_id;
    uint64_t pid1, pid2;
    int lock;
    char buffer[BUFFER_SIZE];
    unsigned int nread;  //number of bytes read
    unsigned int nwrite; //number of bytes written
    //el incluye cantidad de bytes a leer y a escribir
    struct pipe * next;
} pipe;

extern uint64_t _xchg(int * lock, int n);
//
pipe * create_pipe();
pipe * get_pipe(uint64_t id);
void unblock_pipe_partner(uint64_t pipe_id);
int kill_pipe(uint64_t id);
int pipe_write(uint64_t pipe_id, char * addr, int n);
int pipe_read(uint64_t pipe_id,char* addr/*,int n*/);
void print_all_pipes();