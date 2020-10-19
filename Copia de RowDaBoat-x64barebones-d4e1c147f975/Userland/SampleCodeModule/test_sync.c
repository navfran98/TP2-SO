#include <stdint.h>
#include "stdio.h"
#include <syscalls.h>
#include <semaphores_manager.h>

#define TOTAL_PAIR_PROCESSES 3

extern char* num_to_string(int num);
extern void _yield();
uint64_t global;  //shared memory

void slowInc(uint64_t *p, uint64_t inc, uint64_t with_sem){
   int64_t aux = *p;
  //  if(with_sem == 0){
   //_yield();
   syscall_force_new_selection();
  //  }
   aux += inc;
   *p = aux;
   
}

void inc_sem(){
    uint64_t i;
    int64_t value = 1;
    uint64_t N = 5;
    //uint16_t sem_id = create_sem(1);
    

    sem * s = create_sem(1);
    uint64_t pid = syscall_get_pid();
    for (i = 0; i < N; i++){

        // print(num_to_string(global));
        // print("\n");
        sem_wait(s,pid);

        slowInc(&global, value, 1);

        sem_post(s, pid);

    }
    

    if(kill_sem(s->sem_id) == 0){
        print("Error en kill\n");
    }

    print("\nFinal value: ");
    print(num_to_string(global));
    print("\n");
    // syscall_ps();
    // syscall_kill(5);
    // syscall_ps();
    syscall_kill(pid);
    
}


void inc_no_sem(uint64_t sem ){
  uint64_t i;
  uint64_t N = 50;
  
  for (i = 0; i < N; i++){
    print("pid: ");
    print(num_to_string(syscall_get_pid()));
    print("num: "); 
    print(num_to_string(global));
    print("\n"); 
    slowInc(&global, 1, 0);
  }

  print("Final value: ");
  print(num_to_string(global));
  print("\n");

  print("Sale seppuku, mi pid es: ");
  print(num_to_string(syscall_get_pid()));
  print("\n");
  syscall_ps();
  //syscall_kill(syscall_get_pid());
  
}

void test_sync(){
  uint64_t i;
  global = 0;

  print("CREATING PROCESSES...(WITH SEM)\n");

  for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    syscall_create_process("inc_sem", &inc_sem, 5, 0);
    syscall_create_process("inc_sem", &inc_sem, 3, 0);
  }
}

void test_no_sync(){
  uint64_t i;

  global = 0;

  print("CREATING PROCESSES...(WITHOUT SEM)\n");

  for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    syscall_create_process("inc_no_sem", &inc_no_sem, 1, 0);
    syscall_create_process("inc_no_sem", &inc_no_sem, 1, 0);
  }
}

