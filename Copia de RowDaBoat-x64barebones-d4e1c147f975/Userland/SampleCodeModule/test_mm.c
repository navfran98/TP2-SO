#include <stdio.h>
#include <string.h>
#include "test_util.h"

//si pongo 10 y 1024 nomas, funka..................... O.o

#define MAX_BLOCKS 10
#define MAX_MEMORY 1024 * 3  //Should be around 80% of memory managed by the MM

#define NULL ((void*)0)

typedef struct MM_rq{
  void * address;
  uint32_t size;
}mm_rq;

void test_mm(){
  mm_rq mm_rqs[MAX_BLOCKS];
  uint8_t rq;
  uint32_t total;

  while (1){
    rq = 0;
    total = 0;

        
    // Request as many blocks as we can
    print("Request\n");
    while(rq < MAX_BLOCKS && total < MAX_MEMORY){
      mm_rqs[rq].size = GetUniform(MAX_MEMORY - total - 1) + 1;
      mm_rqs[rq].address = syscall_buddy_malloc(mm_rqs[rq].size); // TODO: Port this call as required
      //TODO: check if NULL
      if(mm_rqs[rq].address == NULL){
        print("ERROR!!!!!!\n");
      }      
      total += mm_rqs[rq].size;
      rq++;
    }
  
    // Set
     print("Set\n");
    uint32_t i;
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address != NULL)
        memset(mm_rqs[i].address, i, mm_rqs[i].size); // TODO: Port this call as required
    
    // Check
     print("Check\n");
    for (i = 0; i < rq; i++){
      if (mm_rqs[i].address != NULL){
        if(!memcheck(mm_rqs[i].address, i, mm_rqs[i].size)){
          print("ERROR!\n");
          return;// TODO: Port this call as required
        }
      }
    }
    // Free
     print("Free\n");
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address != NULL)
        syscall_buddy_free(mm_rqs[i].address);  // TODO: Port this call as required
  } 
}

// int main(){
//   test_mm();
//   return 0;
// }