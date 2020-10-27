#include <stdint.h>

#define NULL (void *) 0
#define MAX_LEVELS 10
#define BLOCKS_PER_LEVEL(lvl) (1<<(lvl))
#define SIZE_OF_BLOCKS_AT_LEVEL(lvl,total_size) ((total_size) / (1<<(lvl)))
#define INDEX_OF_POINTER_IN_LEVEL(pointer,lvl,memory_start,total_size) \
   (((pointer)-(memory_start)) / (SIZE_OF_BLOCKS_AT_LEVEL(lvl, total_size)))
#define TREE_ADDRESS ((void *) 0x0000000000050000L)


enum state {OCCUPIED, FREE, PARTIALLY_OCCUPIED};

typedef struct Tlevel{
   void * starting_address;

   struct Tlevel * left;
   struct Tlevel * right;
   struct Tlevel * parent;

   enum state state;

   uint8_t level;
} block;

typedef struct buddy_list_header{
   block * root;
   uint64_t * base_mem;
   uint64_t total_mem;
   uint64_t free_mem;
   uint64_t used_mem;

} buddy_list_header; 

void generate_buddy_tree(void * base_mem, uint64_t total_mem);
void * buddy_malloc(uint64_t size);
void buddy_free(void * ptr);
void buddy_check_mem_state();