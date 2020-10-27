#include <stdio.h>
#include "test_util.h"
#include <syscalls.h>
#include <string.h>

//TO BE INCLUDED
void endless_loop(){
  while(1);
}

#define MAX_PROCESSES 10 //Should be around 80% of the the processes handled by the kernel

enum State {ERROR, RUNNING, BLOCKED, KILLED};

typedef struct P_rq{
  uint32_t pid;
  enum State state;
}p_rq;

void test_processes(){
  p_rq p_rqs[MAX_PROCESSES];
  uint8_t rq;
  uint8_t alive = 0;
  uint8_t action;
  
  while (1){

    print("ENTRANDO AL WHILEEEEEEEE\n");

    // Create MAX_PROCESSES processes
    print("Creating processes...\n");
    for(rq = 0; rq < MAX_PROCESSES; rq++){
      p_rqs[rq].pid = syscall_create_process("endless_loop", &endless_loop, -1, 0);  // TODO: Port this call as required

      if (p_rqs[rq].pid == -1){                           // TODO: Port this as required
        print("Error creating process\n");               // TODO: Port this as required
        return;
      }else{
        p_rqs[rq].state = RUNNING;
        alive++;
      }
    }

    
    // Randomly kills, blocks or unblocks processes until every one has been killed
    while (alive > 0){

      print("KILLING/BLOCKING PROCESSES\n");
      for(rq = 0; rq < MAX_PROCESSES; rq++){
        action = GetUniform(2) % 2; 

        switch(action){
          case 0:
            if (p_rqs[rq].state == RUNNING || p_rqs[rq].state == BLOCKED){
              if (syscall_kill(p_rqs[rq].pid) == 0){          // TODO: Port this as required
                print("Error killing process\n");        // TODO: Port this as required
                return;
              }
              p_rqs[rq].state = KILLED; 
              alive--;
            }
            break;

          case 1:
            if (p_rqs[rq].state == RUNNING){
              if(syscall_block(p_rqs[rq].pid) == 0){          // TODO: Port this as required
                print("Error blocking process\n");       // TODO: Port this as required
                return;
              }
              p_rqs[rq].state = BLOCKED; 
            }
            break;
        }
      }

      // Randomly unblocks processes
      print("UNBLOCKING PROCESSES\n");
      for(rq = 0; rq < MAX_PROCESSES; rq++)
        if (p_rqs[rq].state == BLOCKED && GetUniform(2) % 2){
          if(syscall_unblock(p_rqs[rq].pid) == 0){            // TODO: Port this as required
            print("Error unblocking process\n");         // TODO: Port this as required
            return;
          }
          p_rqs[rq].state = RUNNING; 
        }
    }
    print("All processes have been killed\n"); 
  }
}
