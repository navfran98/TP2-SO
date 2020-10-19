#include <stdint.h>

//falta incluir las structs aca
void * buddy_malloc(uint64_t size);
void buddy_free(void * ptr);
void buddy_check_mem_state();
// void occupy_children(block * block);
// void free_children(block * block);
// void update_states(block * node);
// uint64_t _generate_children(block * parent, buddy_list_header buddy_tree, uint64_t i);
// void * _alloc(uint8_t level, block * ode);
// void printTabs(int level);
// void printTree(block * root);
// void printState(block* n);