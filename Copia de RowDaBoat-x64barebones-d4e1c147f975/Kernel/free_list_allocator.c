#include <screen_driver.h> 
#include <free_list_allocator.h>
#include <stdint.h>
#include <lib.h>

#define NULL (void *) 0

//mover las estructuras al .h
typedef struct mem_slot{
	struct mem_slot * next;
	struct mem_slot * prev;
	uint8_t * location;
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

	//we create the actual first slot, which size is the de whole memory the mm works with
	slot first;
	first.size = total_mem;
	first.location = (uint8_t *) base_mem;
	first.next = NULL;
	first.prev = NULL;

	memcpy(first.location, &first, sizeof(slot));
}

void * free_list_malloc(uint64_t bytes_to_alloc){

	uint64_t units = bytes_to_alloc + sizeof(slot);
	if(units > mem.free_mem){
		drawString("el espacio libre no alcanza\n");
		return;
	}

	slot * aux = mem.list;

	for(; aux != NULL; aux = aux->next){
		if(aux->size >= units){
			if(aux->size == units){
				if(aux->prev != NULL){
					aux->prev->next = aux->next;
					aux->next->prev = aux->prev;
				} else{
					mem.list = aux->next;
					aux->next->prev = NULL;
				}	
			} else{
				slot node;
				node.next = aux->next;
				node.prev = aux->prev;
				node.location = aux->location + units;
				node.size = aux->size - units;
				// myMemCpy(aux+units, &node, sizeof(slot));
				memcpy(node.location, &node, sizeof(slot));
		
				if(aux->prev == NULL){ //when you only have one node
					mem.list = (slot *) node.location;
				}else{
					aux->prev->next = (slot *) node.location;
				}
			}

			aux->size = units;
			aux->next = mem.used;       
			aux->prev = NULL;
			mem.used = aux;

			mem.free_mem -= units;
			mem.used_mem += units;

			return (void *) (aux->location + sizeof(slot));
		}
	}

	return NULL;
}


void free_list_free(void * ptr){

	uint8_t * aux_ptr = (uint8_t *)ptr - sizeof(slot);
	slot * used_iterator = mem.used;
	slot * used_iterator_prev = used_iterator;
	
	//search the node we have to include in the freelist
	for(; used_iterator != NULL && used_iterator->location != aux_ptr; used_iterator_prev = used_iterator, used_iterator = used_iterator->next){

	}
	
	//si no lo encuentro me pasaron cualquier cosa para hacerle free
	if(used_iterator == NULL){
		drawString("No lo encontro\n");
		return;		
	}
	
	slot * freep = mem.list;
	slot * freep_prev = freep;
	slot * block = used_iterator;

	//arreglo los punteros de la lista oculta
	if(mem.used == block){
		mem.used = block->next;
	} else {
		used_iterator_prev->next = block->next;
	}

	//search where to include de new node en the freelist
	for(; freep != NULL && freep < block; freep_prev = freep, freep = freep->next){
	}
	
	if(freep == freep_prev){
		mem.list = block;
		block->next = freep;
		block->prev = NULL;
		if(freep != NULL){
			freep->prev = block;
		}

	} else {
		//esta entre freep_prev y freep
		block->next = freep;
		block->prev = freep_prev;
		freep_prev->next = block;
		if(freep != NULL){
			freep->prev = block;
		}
		
	}
	//chequear si hay que unir bloques
	if(block->next != NULL && block + block->size == block->next){
			block->size += block->next->size;
			block->next = block->next->next;
	}

	if(block->prev != NULL && block->prev + block->prev->size == block){
		block->prev->size += block->size;
		block->prev->next = block->next;
	} 

	mem.free_mem += block->size;
	mem.used_mem -= block->size;
}

void free_list_check_mem_state(uint64_t * state){
	state[0] = mem.total_mem;
	state[1] = mem.free_mem;
	state[2] = mem.used_mem;
}






























