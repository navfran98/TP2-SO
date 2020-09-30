#include <screen_driver.h> 
#include <buddy_allocator.h>
#include <stdint.h>
#include <lib.h>

#define NULL (void *) 0
#define MAX_LEVELS 26 /* 2^26 = 64MB. */
#define BLOCKS_PER_LEVEL(lvl) (1<<(lvl))
#define SIZE_OF_BLOCKS_AT_LEVEL(lvl,total_size) ((total_size) / (1<<(lvl)))
#define INDEX_OF_POINTER_IN_LEVEL(pointer,lvl,memory_start,total_size) \
   (((pointer)-(memory_start)) / (SIZE_OF_BLOCKS_AT_LEVEL(lvl, total_size)))

static void * base_mem = (void *) 0x700000;  
static uint64_t total_mem = 1024 * 1024 * 64;
static uint64_t * tree_base_address = (uint64_t *) 0x900000;



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

static buddy_list_header buddy;

void occupy_children(block * block);
void free_children(block * block);
void update_states(block * node);
void _generate_children(block * parent, buddy_list_header buddy_tree);

void generate_buddy_tree(void * base_mem, uint64_t total_mem){

   buddy_list_header * buddy_tree = (buddy_list_header *) tree_base_address;
   buddy_tree->base_mem = (uint64_t*)base_mem;
   buddy_tree->total_mem = total_mem;
   buddy_tree->free_mem = total_mem;
   buddy_tree->used_mem = 0;

   block root_node; 
   root_node.state = FREE;
   root_node.level = 0;
   root_node.parent = NULL;
   memcpy(buddy_tree->root, &root_node, sizeof(block));
   
   _generate_children(buddy_tree->root, *buddy_tree);
   
}

void _generate_children(block * parent, buddy_list_header buddy_tree){
   if(parent->level < MAX_LEVELS){
      block left_block;
      left_block.starting_address = parent->starting_address;
      left_block.level = parent->level +1;
      left_block.state = FREE;
      left_block.parent = parent;

      memcpy(parent->left, &left_block, sizeof(block));

      block right_block;
      right_block.starting_address = parent->starting_address  + SIZE_OF_BLOCKS_AT_LEVEL(parent->level + 1,buddy_tree.total_mem);
      right_block.level = parent->level + 1;
      right_block.state = FREE;
      right_block.parent = parent;

      memcpy(parent->right, &right_block,sizeof(block));

      _generate_children(parent->left, buddy_tree);
      _generate_children(parent->right, buddy_tree);
   }

   return;

}

uint8_t get_level(uint8_t size){
   buddy_list_header * buddy_tree = (buddy_list_header *) tree_base_address;
   if(size > buddy_tree->free_mem)
      return -1;

   int level = 0;
   int aux = total_mem;  
            
   while(aux >= size && level < MAX_LEVELS){
      aux /= 2;
      level++;
   }
   return level - 1;
}

void * buddy_malloc(uint64_t size){
   uint8_t level = get_level(size);
   //get_level me devuelve -1 si ese tamaño es más grande que el tamaño que tengo disponible
   if(level != -1){
      buddy_list_header * buddy_tree = (buddy_list_header *) tree_base_address;
      block* aux = buddy_tree->root;

      if(buddy_tree->root->state == FREE){
         while(level != aux->level){
            aux = aux->left;
         }
      }
      else{ 

         while(aux->level != level){
            if(aux->left->state == PARTIALLY_OCCUPIED || aux->left->state == FREE){      
               aux = aux->left;
            }
            else{
               aux = aux->right;
            }
         }
      }
      aux->state = OCCUPIED;
      buddy_tree->free_mem -= SIZE_OF_BLOCKS_AT_LEVEL(aux->level,total_mem);
      buddy_tree->used_mem += SIZE_OF_BLOCKS_AT_LEVEL(aux->level,total_mem);
      update_states(aux);
      occupy_children(aux);
      return aux->starting_address;
   }
   return NULL;
}



void update_states(block * node){
   block * aux = node->parent;
   while(node != NULL){
      if(node->left->state == FREE && node->right->state == FREE){
         node->state = FREE;
      }
      else if(node->left->state == OCCUPIED && node->right->state == OCCUPIED){
         node->state = OCCUPIED;
      }
      else{
         node->state = PARTIALLY_OCCUPIED;
      }
      node = node->parent;
   }
}

void occupy_children(block * block){
   if(block->level < MAX_LEVELS){
      block->left->state = OCCUPIED;
      block->right->state = OCCUPIED;
      occupy_children(block->left);
      occupy_children(block->right);
   }
}
void free_children(block * block){
   if(block->level < MAX_LEVELS){
      block->left->state = FREE;
      block->right->state = FREE;
      occupy_children(block->left);
      occupy_children(block->right);
   }
}


void buddy_free(void * ptr){
   buddy_list_header * tree = (buddy_list_header *) tree_base_address;
   block * node = tree->root;
   if((ptr < node->starting_address) && (ptr > node->starting_address + total_mem)){
      return;
   }

   while(node->starting_address != ptr){
      if(node->level > MAX_LEVELS){
         return;
      }
      node = node->right;
   }
   while(node->state != OCCUPIED){
      if(node->level > MAX_LEVELS){
         return;
      }
      node = node->left;
   }

   node->state = FREE;
   tree->free_mem += SIZE_OF_BLOCKS_AT_LEVEL(node->level,total_mem);
   tree->used_mem -= SIZE_OF_BLOCKS_AT_LEVEL(node->level,total_mem);
   free_children(node);
   update_states(node);
}

void buddy_check_mem_state(uint64_t * state){
   buddy_list_header * tree = (buddy_list_header *) tree_base_address;
   state[0] = tree->total_mem;
   state[1] = tree->free_mem;
   state[2] = tree->used_mem;
}

