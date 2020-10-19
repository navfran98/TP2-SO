#include <stdint.h>
#include "stdio.h"
#include <syscalls.h>
#include <semaphores_manager.h>


#define TOTAL_PAIR_PROCESSES 2
#define SEM_ID "sem"

extern char* num_to_string(int num);

int64_t global;  //shared memory

void slowInc(int64_t *p, int64_t inc){
   int64_t aux = *p;
   aux += inc;
//   yield(); //NO RECONOCE ESTO....... 
//    syscall_force_new_selection();
   *p = aux;
}

// void slowInc(long int *p, int inc){
//   long int aux = *p;
// //   if(state == NO_SEM) {
// //     for(uint64_t i=0; i<100; i++){ /* busy waiting */ }
// //   }
//   aux += inc;
//   *p = aux;
// }


void inc_sem(){
    uint64_t i;
    int64_t value = 1;
    uint64_t N = 5;
    //uint16_t sem_id = create_sem(1);
    

    sem * s = create_sem(1);
  
    for (i = 0; i < N; i++){
        print("EL VALOR DEL LOCK ANTES DEL WAIT ES: ");
        print(num_to_string(s->lock));
        print("\n");
        sem_wait(s, syscall_get_pid());
        print("EL VALOR DEL LOCK DESPUES DEL WAIT ES: ");
        print(num_to_string(s->lock));
        print("\n");

        slowInc(&global, value);

        print("EL VALOR DEL LOCK ANTES DEL POST ES: ");
        print(num_to_string(s->lock));
        print("\n");
        sem_post(s, syscall_get_pid());
        print("EL VALOR DEL LOCK DESPUES DEL POST ES: ");
        print(num_to_string(s->lock));
        print("\n");



        print("termine un ciclo\n");
    }
    print("CHAUUUU\n");

    if(kill_sem(s->sem_id) == 0){
        print("Error en kill\n");
    }

    print("Final value: ");
    print(num_to_string(global));
    print("\n");
  //print("Final value: %d\n", global);
}

void inc_no_sem(uint64_t sem ){
  uint64_t i;
  uint64_t N = 100000;
  
  for (i = 0; i < N; i++){
    slowInc(&global, 1);
  }
  
    print("Final value: ");
    print(num_to_string(global));
    print("\n");
}

void test_sync(){
  uint64_t i;

  global = 0;

  print("CREATING PROCESSES...(WITH SEM)\n");

  for(i = 0; i < 2; i++){
    syscall_create_process("inc_sem", &inc_sem, 5, 0);
    syscall_create_process("inc_sem", &inc_sem, 5, 0); //Sera porque es la misma direccion?
  }
}

void test_no_sync(){
  uint64_t i;

  global = 0;

  print("CREATING PROCESSES...(WITHOUT SEM)\n");

  for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    syscall_create_process("inc_no_sem", &inc_no_sem, 5, 0);
    syscall_create_process("inc_no_sem", &inc_no_sem, 5, 0);
  }
}

