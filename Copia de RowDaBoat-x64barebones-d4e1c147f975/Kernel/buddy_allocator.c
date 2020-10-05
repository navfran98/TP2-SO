#include <screen_driver.h> 
#include <buddy_allocator.h>
#include <stdint.h>
#include <lib.h>

#define NULL (void *) 0
#define MAX_LEVELS 4 
#define BLOCKS_PER_LEVEL(lvl) (1<<(lvl))
#define SIZE_OF_BLOCKS_AT_LEVEL(lvl,total_size) ((total_size) / (1<<(lvl)))
#define INDEX_OF_POINTER_IN_LEVEL(pointer,lvl,memory_start,total_size) \
   (((pointer)-(memory_start)) / (SIZE_OF_BLOCKS_AT_LEVEL(lvl, total_size)))
#define TREE_ADDRESS ((void *) 0x0000000000050000L)

static uint64_t * block_base_address = (uint64_t *) (0x900000);
static uint64_t * tree_base_address = (uint64_t *) TREE_ADDRESS;
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

void occupy_children(block * block);
void free_children(block * block);
void update_states(block * node);
uint64_t _generate_children(block * parent, buddy_list_header buddy_tree, uint64_t i);
void * _alloc(uint8_t level, block*node);

void printTabs(int level);
void printTree(block * root);
void printState(block* n);

void generate_buddy_tree(void * base_mem, uint64_t total_mem){
   
   drawNumber(sizeof(block), 124124214, 32);
   buddy_list_header * buddy_tree = (buddy_list_header *) tree_base_address;
   
   buddy_tree->base_mem = (uint64_t*)base_mem;
   buddy_tree->total_mem = total_mem;
   buddy_tree->free_mem = total_mem;
   buddy_tree->used_mem = 0;

   buddy_tree->root = (block*) block_base_address;
  
   buddy_tree->root->state = FREE;
   buddy_tree->root->level = 0;
   buddy_tree->root->parent = NULL;
   buddy_tree->root->left = (block*) block_base_address + 1;
   buddy_tree->root->right = (block*) block_base_address + 2;
   buddy_tree->root->starting_address = base_mem; 

   int i = _generate_children(buddy_tree->root, *buddy_tree,3);
   block * aux = (block*) block_base_address;
   aux = (block*) block_base_address;
   buddy_tree->root = aux;
}

uint64_t _generate_children(block * parent, buddy_list_header buddy_tree, uint64_t i){

   if(parent->level < MAX_LEVELS-1){
      parent->left->starting_address = parent->starting_address;
      parent->left->level = parent->level + 1;
      parent->left->state = FREE;
      parent->left->parent = parent;
      parent->left->right = (block *) block_base_address + i;
      i++;
      parent->left->left = (block *) block_base_address +  i;
      i++;
   
      parent->right->starting_address = (void * ) parent->starting_address  + SIZE_OF_BLOCKS_AT_LEVEL(parent->level + 1, buddy_tree.total_mem);
      parent->right->level = parent->level + 1;
      parent->right->state = FREE;
      parent->right->parent = parent;
      parent->right->right = (block*)block_base_address + i;
      i++;
      parent->right->left = (block*)block_base_address +  i;
      i++;

      i = _generate_children(parent->left, buddy_tree,i);
      i = _generate_children(parent->right, buddy_tree,i);
   }
   else{
      parent->left->starting_address = parent->starting_address;
      parent->left->level = parent->level + 1;
      parent->left->state = FREE;
      parent->left->parent = parent;
      parent->left->right = NULL;
      parent->left->left = NULL;
   
      parent->right->starting_address = parent->starting_address  + SIZE_OF_BLOCKS_AT_LEVEL(parent->level + 1, buddy_tree.total_mem);
      parent->right->level = parent->level + 1;
      parent->right->state = FREE;
      parent->right->parent = parent;
      parent->right->right = NULL;
      parent->right->left = NULL;
   }
   return i;
}

uint8_t get_level(uint64_t size){
   buddy_list_header * buddy_tree = (buddy_list_header *) tree_base_address;
   drawNumber(size,412412,4);
   drawString("<-size  free mem->");
   drawNumber(buddy_tree->free_mem,3123123,33);

   if(size > buddy_tree->free_mem)
      return -1;

   int level = 0;
   int aux = buddy_tree->total_mem;  
            
   while(aux > size && level < MAX_LEVELS){
      aux /= 2;
      level++;
   }
   return level;
}

void * buddy_malloc(uint64_t size){
   uint8_t level = get_level(size);

   //get_level me devuelve -1 si ese tamaño es más grande que el tamaño que tengo disponible
   if(level != -1){
      buddy_list_header * buddy_tree = (buddy_list_header *) tree_base_address;
      void * to_ret = _alloc(level,buddy_tree->root); 
      if(to_ret != NULL){
         buddy_tree->free_mem -= SIZE_OF_BLOCKS_AT_LEVEL(level, buddy_tree->total_mem);
         buddy_tree->used_mem += SIZE_OF_BLOCKS_AT_LEVEL(level, buddy_tree->total_mem);
      }
      return to_ret;
      //return _alloc(level, buddy_tree->root);
   }
   return NULL;
}

void * _alloc(uint8_t level, block*node){
   if(node->level == level){
      if(node->state == FREE){
         node->state = OCCUPIED;
         occupy_children(node);
         update_states(node);
         return node->starting_address;
      }
      else
         return NULL;
   }
   else{
      if(node->state == OCCUPIED){
         return NULL;
      }
      else
      {
         void * left_alloc = _alloc(level, node->left);
         if(left_alloc == NULL){
            return _alloc(level, node->right);
         }
         return left_alloc;
      }
      
   }
}

void update_states(block * node){
   block * aux = node->parent;
   while(aux != NULL){
      if(aux->left->state == FREE && aux->right->state == FREE){
         aux->state = FREE;
      }
      else if(aux->left->state == OCCUPIED && aux->right->state == OCCUPIED){
         aux->state = OCCUPIED;
      }
      else{
         aux->state = PARTIALLY_OCCUPIED;
      }
      aux = aux->parent;
   }
}

void occupy_children(block * block){
   if(block->right != NULL){
      block->left->state = OCCUPIED;
      block->right->state = OCCUPIED;
      occupy_children(block->left);
      occupy_children(block->right);
   }
}

void free_children(block * block){
   if(block->right != NULL){
      block->left->state = FREE;
      block->right->state = FREE;
      free_children(block->left);
      free_children(block->right);
   }
}

void buddy_free(void * ptr){
   buddy_list_header * tree = (buddy_list_header *) tree_base_address;
   block * node = tree->root;

   while(node->starting_address != ptr){
      if(node->left == NULL && node->right == NULL){
         drawString("\nLlegue al final y no encontre nada\n");
         return;
      }
      if(node->right->starting_address > ptr){
         node = node->left;
      }
      else {
         node = node ->right;
      }
   }
   while(node->state != OCCUPIED){
      if(node->left == NULL){
         drawString("\nLlegue al final y no encontre nada\n");
         return;
      }
      node = node->left;
   }

   node->state = FREE;
   tree->free_mem += SIZE_OF_BLOCKS_AT_LEVEL(node->level,tree->total_mem);
   tree->used_mem -= SIZE_OF_BLOCKS_AT_LEVEL(node->level,tree->total_mem);
   free_children(node);
   update_states(node);
   drawString("\nHago free de: ");
   drawNumber(node->starting_address);
}

void buddy_check_mem_state(uint64_t * state){
   buddy_list_header * tree = (buddy_list_header *) tree_base_address;
   state[0] = tree->total_mem;
   state[1] = tree->free_mem;
   state[2] = tree->used_mem;

   drawString("Total mem: ");
   drawNumber(tree->total_mem, 33333,3);
   drawString("\nFree mem: ");
   drawNumber(tree->free_mem, 33333,3);
   drawString("\nUsed mem: ");
   drawNumber(tree->used_mem, 33333,3);
}

void printTabs(int level){
	if (level == 0)
		return;
	while (level !=0 ){
		drawString("I");
		drawString("       ");
		level--;
	}
	drawString("I->");
}

void printState(block * n){
	if (n->state == FREE)
		drawString("FREE\n");
	else if (n->state == OCCUPIED)
		drawString("OCCUPIED\n");
	else if (n->state == PARTIALLY_OCCUPIED){
		drawString("PARTIALLY OCCUPIED\n");
	}
}

void printTree (block * root){
	if (root == NULL)
		return;
	printTabs(root->level);
	printState(root);
	printTree(root->left);
	printTree(root->right);
}