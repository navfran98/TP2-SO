char * base_mem = 0x5000000; //verificar puntero de comienzo
unsigned total_mem = 1024 * 1024 * 10, free_mem, used_mem;

typedef struct header{
	struct node * next;
	unsigned size;
} Theader;


static Theader free_list;
static Theader * freep = NULL;

void * my_malloc(unsigned size_bytes){

	Theader * prev, * p;

	if((prev = freep) == NULL){
		free_list->next = freep = prev = base_mem;
		free_list->size = total_mem;
		// free(base->next);
		// base.next = prev = freep = &base;
		// free_mem = total_mem;
		// used_mem = 0;
	}

	for(p = prev->next; ; prev = p, p = p->next){
		if(p->size >= size_bytes){
			if(p->size == size_bytes){
				prev->next = p->next;
			}else{
				p->size -= size_bytes;
				p += p->size;
				p->size = syze_bytes;
			}
			freep = prev;
			return (void *) (p+1);
		}
		if(p == freep){
			return NULL;
		}
	}
}

void my_free(void * ptr){

    Theader *aux_ptr, *p;
	aux_ptr = (Theader *) ptr - 1;

	for (p = freep; !(aux_ptr > p && aux_ptr < p->ptr); p = p->ptr){
		if (p >= p->ptr && (aux_ptr > p || aux_ptr < p->ptr)) 
			break;
		//busca el bloque p, en el cual esta la direccion que quiero hacerle free
	}
	//caso que aux_ptr esta el final del bloque p
	if (aux_ptr + aux_ptr->size == p->ptr) {
		aux_ptr->size += p->ptr->size;
		aux_ptr->ptr = p->ptr->ptr;
	} else
		aux_ptr->ptr = p->ptr; 
	//caso que aux_ptr este al principio del bloque p
	if (p + p->size == aux_ptr) {
		p->size += aux_ptr->size;
		p->ptr = aux_ptr->ptr; 
	} else
		p->ptr = aux_ptr; 
	freep = p;
}





