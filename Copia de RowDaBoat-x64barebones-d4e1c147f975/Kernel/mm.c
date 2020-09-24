#include <screen_driver.h> 
#include <mm.h>

#define NULL (void *) 0



unsigned total_mem = 1024 * 1024 * 10, free_mem, used_mem;

typedef struct header{ 
	struct header * next;
	unsigned size;
} Theader;

static void * base_mem = (void *)0x5000000; //verificar puntero de comienzo  


static Theader free_list;
static Theader * freep = NULL;

void * my_malloc(unsigned size_bytes){

	Theader * prev, * p;


	if((prev = freep) == NULL){
		free_list.next = freep = prev = &free_list;
		free_list.size = total_mem;
		my_free(&free_list);
		drawString("ENtro aca\n");
		// free_list.size = prev->next->size = total_mem;
		// free(base->next);
		// free_mem = total_mem;
		// used_mem = 0;
	}

	// unsigned nunits = (size_bytes + sizeof(Theader) - 1)/sizeof(Theader) + 1;

	unsigned nunits = size_bytes;
	for(p = prev->next; ; prev = p, p = p->next){

		if(p->size == total_mem){
			drawString("p tiene una memoria de: Total_Mem\n");
		}

		if(p->size == 0){
			drawString("p tiene una memoria de: Cero\n");
		}

		if(p->size >= nunits){
			drawString("SI Hay espacio en este bloque\n");
			 if(p->size == nunits){
			// 	drawString("size == nunits\n");
				p->size -= nunits;
			 	p += p->size;
			 	p->size = nunits;
			 	prev->next = p->next; //lo elimina a p"""
			 }else{
			// 	drawString("lo toy metiendo\n");
			 	p->size -= nunits;
			 	p += p->size;
			 	p->size = nunits;
			}
			drawString("retorno la direccion\n");
			freep = prev;
			return (void *) (p+1);
		}
		if(p == freep){
			drawString("Di una vuelta completa y chau\n");
			return NULL;
		}
		drawString("NO HAY ESPACIO en este bloque\n");
	}
	
}

void my_free(void * ptr){

    Theader *aux_ptr, *p;
	aux_ptr = (Theader *) ptr - 1;

	for (p = freep; !(aux_ptr > p && aux_ptr < p->next); p = p->next){
		if (p >= p->next && (aux_ptr > p || aux_ptr < p->next)) 
			break;
		//busca el bloque p, en el cual esta la direccion que quiero hacerle free
	}
	//caso que aux_ptr esta el final del bloque p
	if (aux_ptr + aux_ptr->size == p->next) {
		aux_ptr->size += p->next->size;
		aux_ptr->next = p->next->next;
	} else
		aux_ptr->next = p->next; 
	//caso que aux_ptr este al principio del bloque p
	if (p + p->size == aux_ptr) {
		p->size += aux_ptr->size;
		p->next = aux_ptr->next; 
	} else
		p->next = aux_ptr; 
	freep = p;
}




