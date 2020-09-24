#include <screen_driver.h> 

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

		free_list.next = freep = prev->next = base_mem;
		free_list.size = prev->next->size = total_mem;
		// free(base->next);
		// base.next = prev = freep = &base;
		// free_mem = total_mem;
		// used_mem = 0;
	}

	unsigned nunits = (size_bytes + sizeof(Theader) - 1)/sizeof(Theader) + 1;

	drawString(nunits);


	for(p = prev->next; ; prev = p, p = p->next){

		if(p == (void*) 0x5000000){
			drawString((char*) &p);
		}
		
		if(p->size >= nunits){
			drawString("SI Hay espacio en este bloque\n");
			
			if(p->size == nunits){
				drawString("size == nunits\n");
				prev->next = p->next;
			}else{
				drawString("lo toy metiendo\n");
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




