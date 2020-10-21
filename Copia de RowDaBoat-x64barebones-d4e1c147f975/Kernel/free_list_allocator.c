#include <screen_driver.h> 
#include <free_list_allocator.h>
#include <stdint.h>
#include <lib.h>

#define NULL (void *) 0

//mover las estructuras al .h
typedef struct mem_slot{
	struct mem_slot * next;
	struct mem_slot * prev;
	void * location;
	uint64_t size;
} slot;

typedef struct header{
	slot * list;
	slot * used;
	uint64_t used_mem;
	uint64_t free_mem;
	uint64_t total_mem;
} list_header;

static list_header mem;

void free_list_init(void * base_mem, uint64_t total_mem){
	mem.total_mem = total_mem;
	mem.free_mem = total_mem;
	mem.used_mem = 0;
	mem.list = (slot *) base_mem;
	mem.used = NULL;

	slot * first = mem.list;
	first->size = total_mem;
	first->location = base_mem;
	first->next = NULL;
	first->prev = NULL;

	// mem.list->location = (uint64_t) base_mem;
	// mem.list->next = NULL;
	// mem.list->prev = NULL;	

	//we create the actual first slot, which size is the de whole memory the mm works with
	// slot first;
	// first.size = total_mem;
	// first.location = (uint8_t *) base_mem;
	// first.next = NULL;
	// first.prev = NULL;

	// memcpy(first.location, &first, sizeof(slot));
}

void * free_list_malloc(uint64_t bytes_to_alloc){

	uint64_t units = bytes_to_alloc + sizeof(slot);
	if(units > mem.free_mem){
		drawString("El espacio libre no alcanza\n");
		return NULL;
	}

	
	for(slot * aux = mem.list; aux != NULL; aux = aux->next){

		if(aux->size >= units){
			if(aux->size == units){
				if(aux->prev != NULL){
					aux->prev->next = aux->next;
				} else{
					mem.list = aux->next;
				}
				if(aux->next != NULL){
					aux->next->prev = aux->prev;
				}
			} else{
				slot * node = aux->location + units;
				node->next = aux->next;
				node->prev = aux->prev;
				node->location = aux->location + units;
				node->size = aux->size - units;

				// node.next = aux->next;
				// node.prev = aux->prev;
				// node.location = aux->location + units;
				// node.size = aux->size - units;
				// // myMemCpy(aux+units, &node, sizeof(slot));
				// memcpy(node.location, &node, sizeof(slot));
		
				if(aux->prev == NULL){ //when you only have one node
					mem.list = node;
				}else{
					aux->prev->next = node;
				}

				if(aux->next != NULL){
					aux->next->prev = node;
				}
			}

			aux->size = units;
			aux->next = mem.used;       
			aux->prev = NULL;
			mem.used = aux;
			if(aux->next != NULL){
				aux->next->prev = aux;	
			}
			mem.free_mem -= units;
			mem.used_mem += units;
	
			return aux->location + sizeof(slot);
		}
	}
	return NULL;
}

void free_list_free(void * ptr){

	void * aux_ptr = ptr - sizeof(slot);
	slot * used_iterator = mem.used;
	
	//search the node we have to include in the freelist
	for(; used_iterator != NULL && used_iterator->location != aux_ptr; used_iterator = used_iterator->next);
	
	//si no lo encuentro me pasaron cualquier cosa para hacerle free
	if(used_iterator == NULL){
		drawString("No lo encontro\n");
		return;		
	}
	
	
	slot * block = used_iterator;

	//arreglo los punteros de la lista oculta
	if(mem.used == block){
		mem.used = block->next;
		// block->next->prev = NULL;
	} else {
		if(used_iterator->prev != NULL){
			used_iterator->prev->next = block->next;
		}
	}

	if(used_iterator->next != NULL){
		used_iterator->next->prev = block->prev;
	}
	
	slot * freep;
	slot * freep_prev = NULL;
	//search where to include the new node in the freelist
	for(freep = mem.list; freep != NULL && freep->location < block->location; freep_prev = freep, freep = freep->next);
	
	if(freep == NULL){ // Free list vacia o va al final de la lista
		block->next = NULL;
		if(mem.list == NULL){
			mem.list = block;
			block->prev = NULL;
		} else {
			block->prev = freep_prev;
			freep_prev->next = block;
		}
	} else if(freep->prev == NULL){ // Va al principio de la lista
		mem.list = block;
		block->next = freep;
		block->prev = NULL;
		freep->prev = block;
	} else {
		block->next = freep;
		block->prev = freep->prev;
		freep->prev->next = block;
		freep->prev = block;
	}
	//chequear si hay que unir bloques
	if(block->next != NULL && block->location + block->size == block->next->location){
		block->size += block->next->size;
		block->next = block->next->next;
		block->next->prev = block;
	}

	if(block->prev != NULL && block->prev->location + block->prev->size == block->location){
		block->prev->size += block->size;
		block->prev->next = block->next;
		block->next->prev = block->prev;
		
	} 

	mem.free_mem += block->size;
	mem.used_mem -= block->size;
}

void free_list_check_mem_state(){
   drawString("Total mem: ");
   drawNumber(mem.total_mem, 0xFFFFFF, 0x000000);
   drawString("\nFree mem: ");
   drawNumber(mem.free_mem, 0xFFFFFF, 0x000000);
   drawString("\nUsed mem: ");
   drawNumber(mem.used_mem, 0xFFFFFF, 0x000000);
}






























