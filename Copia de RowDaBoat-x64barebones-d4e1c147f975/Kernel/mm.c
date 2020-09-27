#include <screen_driver.h> 
#include <mm.h>
#include <stdint.h>

#define NULL (void *) 0

static void * base_mem = (void *) 0x800000; //verificar puntero de comienzo  
static unsigned total_mem = 1024 * 1024 * 10;


void initialize_mm();
void * my_memcpy(void * dest, const void * src, uint64_t length);
void * my_malloc(uint8_t bytes_to_alloc);
void my_free(void * ptr);


//https://www.geeksforgeeks.org/write-memcpy/
void myMemCpy(void *dest, void *src, unsigned n) { 
   // Typecast src and dest addresses to (char *) 
   char *csrc = (char *)src; 
   char *cdest = (char *)dest; 
  
   // Copy contents of src[] to dest[] 
   for (int i=0; i<n; i++) 
       cdest[i] = csrc[i]; 
}

typedef struct mslot{
	struct mslot * next;
	struct mslot * prev;
	uint8_t * location;
	unsigned size;
} slot;

typedef struct header{
	slot * list;
	slot * used;
	unsigned used_mem;
	unsigned free_mem;
} list_header;


static list_header mem;



//revisar si es necesario utilizar location

void * my_malloc(uint8_t bytes_to_alloc){
	// if(mem.list == NULL){
	// 	initialize_mm();
	// }
	initialize_mm();

	if(mem.list->location == base_mem){
		drawString("mem.list esta bien creado en base_mem\n");
	}

	uint8_t units = bytes_to_alloc + sizeof(slot); //SIZEOF(SLOT) porque quiero guardarme info sobre 

	slot * aux = mem.list;

	for(; aux != NULL; aux = aux->next){
		if(aux->size >= units){
			if(aux->size == units){
				
			} else{
				slot node;
				node.next = aux->next;
				node.prev = aux->prev;
				node.location = aux->location + units;
				node.size = aux->size - units;
				// myMemCpy(aux+units, &node, sizeof(slot));
				my_memcpy(aux->location, &node, sizeof(slot));
		
				if(aux->prev == NULL){ //when you only have one node
					mem.list = &node;
				}else{
					aux->prev->next = &node;
				}
			}

			aux->size = units;
			aux->next = mem.used;       
			aux->prev = NULL;
			mem.used = aux;

			mem.free_mem -= units;
			mem.used_mem += units;

			if(base_mem == (void *) 0x800000){
				drawString("base_mem no se modifico\n");
			}
			// return (void *) (aux->location + sizeof(slot));
			return base_mem;
		}
	}

	return NULL;
	
}

void initialize_mm(){
	mem.free_mem = total_mem;
	mem.used_mem = 0;
	mem.list = (slot *) base_mem;	
	mem.used = NULL;

	//we create the actual first slot, which size is the de whole memory the mm works with
	slot first;
	first.size = total_mem;
	first.location = (uint8_t *)base_mem;
	first.next = NULL;
	first.prev = NULL;

	my_memcpy(base_mem, &first, sizeof(slot));

}


void my_free(void * ptr){

	uint8_t * aux_ptr = (uint8_t *)ptr;
	slot * used_iterator = mem.used;
	
	if(aux_ptr == (uint8_t *) 0x800000){
		drawString("mierda\n");
	}
	
	if(mem.used->location == aux_ptr){
		drawString("por favor te pido\n");
	}
	// for(; used_iterator != NULL && used_iterator->location != aux_ptr; used_iterator = used_iterator->next){

	// }
	
	// if(used_iterator == NULL){
	// 		drawString("Problema con la dirgggggggggggggec\n");
			
	// }
	// else{
	// 	drawString("lo encontré!\n");
	// }


	

	// if(mem.used->location != aux_ptr){
	// 	drawString("problema con la direc\n");
	// 	return;
	// }
	
	// slot * freep = mem.list;
	// slot * freep_prev = freep;

	// if(aux_ptr == NULL){
	// 	drawString("NULLLLL\n");
	// }

	// drawString("hola1\n");
	// for(; freep != NULL && freep < aux_ptr; freep_prev = freep, freep = freep->next){
	// 		drawString("g\n");
	// }
    // drawString("Termino el for\n");
	// if(freep == freep_prev){
	// 	mem.list = aux_ptr;
	// 	aux_ptr->next = freep;
	// 	aux_ptr->prev = NULL;
	// 	if(freep != NULL){
	// 		freep->prev = aux_ptr;
	// 	}
	// 	drawString("alfinal del if freep=prev\n");
	// } else {
	// 	//esta entre freep_prev y freep
	// 	aux_ptr->next = freep;
	// 	aux_ptr->prev = freep_prev;
	// 	freep_prev->next = aux_ptr;
	// 	if(freep != NULL){
	// 		freep->prev = aux_ptr;
	// 	}
	// 	drawString("alfinal del else freep=prev\n");
	// }

	// drawString("Antes de unir bloques\n");

	// //chequear si hay que unir bloques
	// if(aux_ptr->next != NULL && aux_ptr + aux_ptr->size == aux_ptr->next){
	// 		aux_ptr->size += aux_ptr->next->size;
	// 		aux_ptr->next = aux_ptr->next->next;
	// }
	
	// drawString("alfinal del if freep=prev\n");
	// if(aux_ptr->prev != NULL && aux_ptr->prev + aux_ptr->prev->size == aux_ptr){
	// 	aux_ptr->prev->size += aux_ptr->size;
	// 	aux_ptr->prev->next = aux_ptr->next;
	// } 

	// mem.free_mem += aux_ptr->size;
	// mem.used_mem -= aux_ptr->size;
}

void check_mem_state(unsigned * state){
	state[0] = total_mem;
	state[1] = mem.free_mem;
	state[2] = mem.used_mem;
}


//https://www.student.cs.uwaterloo.ca/~cs350/common/os161-src-html/doxygen/html/memcpy_8c_source.html
void * my_memcpy(void * destination, const void * source, uint64_t length) {
	/*
	* memcpy does not support overlapping buffers, so always do it
	* forwards. (Don't change this without adjusting memmove.)
	*
	* For speedy copying, optimize the common case where both pointers
	* and the length are word-aligned, and copy word-at-a-time instead
	* of byte-at-a-time. Otherwise, copy by bytes.
	*
	* The alignment logic below should be portable. We rely on
	* the compiler to be reasonably intelligent about optimizing
	* the divides and modulos out. Fortunately, it is.
	*/
	uint64_t i;

	if ((uint64_t)destination % sizeof(uint32_t) == 0 && (uint64_t)source % sizeof(uint32_t) == 0 && length % sizeof(uint32_t) == 0) {
		uint32_t *d = (uint32_t *) destination;
		const uint32_t *s = (const uint32_t *)source;

		for (i = 0; i < length / sizeof(uint32_t); i++)
			d[i] = s[i];
	} else {
		uint8_t * d = (uint8_t*)destination;
		const uint8_t * s = (const uint8_t*)source;

		for (i = 0; i < length; i++)
			d[i] = s[i];
	}

	return destination;
}



//-----------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------






























