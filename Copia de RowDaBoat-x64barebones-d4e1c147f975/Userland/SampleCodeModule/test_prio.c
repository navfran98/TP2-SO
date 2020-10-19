//Comentado porque el endless_loop se repite en otro test

#include <stdint.h>
#include <stdio.h>
#include <syscalls.h>
 
#define MINOR_WAIT 1000000                               // TODO: To prevent a process from flooding the screen
#define WAIT      10000000                              // TODO: Long enough to see theese processes beeing run at least twice


void bussy_wait(uint64_t n){
  uint64_t i;
  for (i = 0; i < n; i++);
}

void endless_loop2(){
  uint64_t pid = syscall_get_pid();

  while(1){
    // print(pid);
    bussy_wait(MINOR_WAIT);
  }
}

#define TOTAL_PROCESSES 3

void test_prio(){
  uint64_t pids[TOTAL_PROCESSES];
  uint64_t i;

  for(i = 0; i < TOTAL_PROCESSES; i++)
    pids[i] = syscall_create_process("endless_loop", &endless_loop2, 5, 0);

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
