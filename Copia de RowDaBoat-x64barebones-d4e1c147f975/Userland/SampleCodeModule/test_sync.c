#include <stdint.h>
#include "stdio.h"
#include <syscalls.h>
#include <semaphores_manager.h>

#define TOTAL_PAIR_PROCESSES 2

extern char* num_to_string(int num);
extern void _yield();
uint64_t global;  //shared memory

void slowInc(uint64_t *p, uint64_t inc, uint64_t with_sem){
  
    int64_t aux = *p;
    if(with_sem == 0) {
      for(uint64_t i=0; i<10000; i++){ /* busy waiting */ }
    }
    _yield();
    aux += inc;
    *p = aux;
}

//WITH SEM
void inc_sem(){
    uint64_t i;
    int64_t value = 1;
    uint64_t N = 10000;
    sem * s = create_sem(1);
    uint64_t pid = syscall_get_pid();
    for (i = 0; i < N; i++){
        sem_wait(s,pid);

        slowInc(&global, value, 1);

        sem_post(s, pid);
    }
    
    if(kill_sem(s->sem_id) == 0){
        print("Error en kill\n");
    }

    print("Final value: ");
    print(num_to_string(global));
    print("\n");
    syscall_kill(pid);
}

void dec_sem(){
    uint64_t i;
    int64_t value = -1;
    uint64_t N = 10000;
    sem * s = create_sem(1);
    uint64_t pid = syscall_get_pid();
    for (i = 0; i < N; i++){
        sem_wait(s,pid);

        slowInc(&global, value, 1);

        sem_post(s, pid);
    }
    
    if(kill_sem(s->sem_id) == 0){
        print("Error en kill\n");
    }

    print("Final value: ");
    print(num_to_string(global));
    print("\n");
    syscall_kill(pid);
}

void test_sync(){
  uint64_t i;
  global = 0;

  print("CREATING PROCESSES...(WITH SEM)\n");

  for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    int pid1, pid2;
    print("Creating process...\n");
    pid1 = syscall_create_process("inc_sem", &inc_sem, 5, 0);
    print("Creating process...\n");
    pid2 = syscall_create_process("dec_sem", &dec_sem, 3, 0);
  }
}

//WITHOUT SEM
void inc_no_sem(){
  uint64_t i;
  uint64_t N = 10000;
  
  for (i = 0; i < N; i++){
    slowInc(&global, 1, 0);
  }

  print("Final value: ");
  print(num_to_string(global));
  print("\n");
  syscall_kill(syscall_get_pid());
}

void dec_no_sem(){
  uint64_t i;
  uint64_t N = 10000;
  
  for (i = 0; i < N; i++){
    slowInc(&global, -1, 0);
  }

  print("Final value: ");
  print(num_to_string(global));
  print("\n");
  syscall_kill(syscall_get_pid());
}

test_no_sync(){
  uint64_t i;

  global = 0;

  print("CREATING PROCESSES...(WITHOUT SEM)\n");

  for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    print("Creating process...\n");
    syscall_create_process("inc_no_sem", &inc_no_sem, 3, 0);
    print("Creating process...\n");
    syscall_create_process("dec_no_sem", &dec_no_sem, 3, 0);
  }
}

