// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//Comentado porque el endless_loop se repite en otro test
#include <stdint.h>
#include <stdio.h>
#include <syscalls.h>
 
#define MINOR_WAIT 1000000                              
#define WAIT      10000000                              


void bussy_wait(uint64_t n){
  uint64_t i;
  for (i = 0; i < n; i++);
}

void endless_loop2(){
  while(1){
    bussy_wait(MINOR_WAIT);
  }
}

#define TOTAL_PROCESSES 3

void test_prio(){
  uint64_t pids[TOTAL_PROCESSES];
  uint64_t i;

  for(i = 0; i < TOTAL_PROCESSES; i++)
    pids[i] = syscall_create_process("endless_loop", &endless_loop2, -1, 0);

  bussy_wait(WAIT);
  print("\nCHANGING PRIORITIES...\n");

  for(i = 0; i < TOTAL_PROCESSES; i++){
    switch (i % 3){
      case 0:
        syscall_set_priority(pids[i], 0); //lowest priority 
        break;
      case 1:
        syscall_set_priority(pids[i], 1); //medium priority
        break;
      case 2:
        syscall_set_priority(pids[i], 2); //highest priority
        break;
    }
  }

  bussy_wait(WAIT);
  print("\nBLOCKING...\n");

  for(i = 0; i < TOTAL_PROCESSES; i++)
    syscall_block(pids[i]);

  print("CHANGING PRIORITIES WHILE BLOCKED...\n");
  for(i = 0; i < TOTAL_PROCESSES; i++){
    switch (i % 3){
      case 0:
        syscall_set_priority(pids[i], 1); //medium priority 
        break;
      case 1:
        syscall_set_priority(pids[i], 1); //medium priority
        break;
      case 2:
        syscall_set_priority(pids[i], 1); //medium priority
        break;
    }
  }

  print("UNBLOCKING...\n");

  for(i = 0; i < TOTAL_PROCESSES; i++)
    syscall_unblock(pids[i]);

  bussy_wait(WAIT);
  print("\nKILLING...\n");

  for(i = 0; i < TOTAL_PROCESSES; i++)
    syscall_kill(pids[i]);
}
