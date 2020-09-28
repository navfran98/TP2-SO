#include <screen_driver.h> 
#include <buddy_allocator.h>
#include <stdint.h>
#include <lib.h>

#define NULL (void *) 0
#define MAX_LEVELS 32 /* 2^32 = 4GB. */
#define BLOCKS_PER_LEVEL(lvl) (1<<(lvl))
#define SIZE_OF_BLOCKS_AT_LEVEL(lvl,total_size) ((total_size) / (1<<(lvl))
#define INDEX_OF_POINTER_IN_LEVEL(pointer,lvl,memory_start,total_size) \
   (((pointer)-(memory_start)) / (SIZE_OF_BLOCKS_AT_LEVEL(lvl, total_size)))

typedef struct Tlevel{
   struct Tlevel * next;
   uint8_t level;
   uint64_t location;
} block;

typedef struct buddy_list_header{
   block ** list;
   uint64_t * base_mem;
   uint64_t total_mem;
   uint64_t free_mem;
   uint64_t used_mem;
} buddy_list_header;

static buddy_list_header buddy;


void buddy_init(void * base_mem, uint64_t total_mem){
   buddy.total_mem = total_mem;
	buddy.free_mem = total_mem;
	buddy.used_mem = 0;
	buddy.list = (block ** ) base_mem;
      
   block first;
   first.level = 0;
   first.next = NULL;
   first.location = (uint64_t *) base_mem;
   
   memcpy(buddy.list[0], &first, sizeof(block));
   
   // SIZE_OF_BLOCKS_AT_LEVEL(first.level, buddy.total_mem)
   for(int i = 0; i < MAX_LEVELS; i++){
      if(i == 0){
         buddy.list[i] = &first;
      } else {
         buddy.list[i] = NULL;
      }
   }
/*    

   |(BLOQUE) 0
   | --> NULL   1
   |--> NULL 2
   |--> NULL  3
   |... 
   |...  
   |...      
   |--> NULL  MAX_LEVELS

*/  
   }

void * buddy_malloc(uint64_t size){

}

void buddy_free(void * ptr){

}




