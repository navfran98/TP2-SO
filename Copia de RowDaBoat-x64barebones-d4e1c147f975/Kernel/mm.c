#include <screen_driver.h> 
#include <mm.h>

#define NULL (void *) 0



static unsigned total_mem = 1024 * 1024 * 10, free_mem, used_mem;

typedef struct header{ 
	struct header * next;
	unsigned size;
} Theader;

static void * base_mem = (void *)0x9000000; //verificar puntero de comienzo  

static Theader free_list;
static Theader * freep = NULL;

//https://www.geeksforgeeks.org/write-memcpy/
void myMemCpy(void *dest, void *src, unsigned n) 
{ 
   // Typecast src and dest addresses to (char *) 
   char *csrc = (char *)src; 
   char *cdest = (char *)dest; 
  
   // Copy contents of src[] to dest[] 
   for (int i=0; i<n; i++) 
       cdest[i] = csrc[i]; 
}




void * my_malloc(unsigned size_bytes){

	Theader * prev, * p;


	if((prev = freep) == NULL){
		myMemCpy(base_mem, &free_list, total_mem);

		free_list.next = freep = prev = &free_list;
		free_list.size = total_mem;
		// my_free(&free_list);
		drawString("Primera vez: Creo mi mm\n");
		free_mem = total_mem;
		used_mem = 0;
	}


	// unsigned nunits = (size_bytes + sizeof(Theader) - 1)/sizeof(Theader) + 1;

	unsigned nunits = size_bytes;
	for(p = prev->next; ; prev = p, p = p->next){
		if(free_mem == 0){
			drawString("No hay memoria libre\n");
			return NULL;
		}

		if(p->size >= nunits){
			drawString("SI, Hay espacio en este bloque\n");
			 if(p->size == nunits){
				
				// p->size -= nunits; //no esta bien porque dsp free necesita saber el size.
				// tenemos que hacer algo para cuando agrega el bloque exacto y que lo saque de la lista
			 	prev->next = p->next; //lo elimina a p"""
			 }else{
			// 	drawString("lo toy metiendo\n");
			 	p->size -= nunits;
			 	p += p->size;
			 	p->size = nunits;
			}
			used_mem += nunits;
			free_mem -= nunits;
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
	drawString("Haciendo un free\n");

    Theader *aux_ptr, *p;
	aux_ptr = (Theader *) ptr;


	for (p = freep; !(aux_ptr > p && aux_ptr < p->next); p = p->next){
		if (p >= p->next && (aux_ptr > p || aux_ptr < p->next)) 
			break;
		//busca el bloque p, en el cual esta la direccion que quiero hacerle free
	}
	
	void * t = (void *) 0x9000000;
	t += 1024 * 1024 * 9;
	if(ptr == t){
		drawString("encontre bien el p\n");
	}

	if(aux_ptr == ptr){
		drawString("qqwerrewq\n");
	}

	t-=1024 * 1024 * 9;
	if(p == ptr){
		drawString("p esta al principio\n");
	}

	if(aux_ptr->size == 1024 * 1024 * 1){
		drawString("El size esta bien\n");
	}

	//caso que aux_ptr esta el final del bloque p
	if (aux_ptr + aux_ptr->size == p->next) {
		drawString("primer if\n");

		aux_ptr->size += p->next->size;
		aux_ptr->next = p->next->next;
	} else
		drawString("primer else\n");
		aux_ptr->next = p->next; 

	
	//caso que aux_ptr este al principio del bloque p
	if (p + p->size == aux_ptr) {
		drawString("Segundo if\n");
		p->size += aux_ptr->size;
		p->next = aux_ptr->next; 
	} else
		drawString("Segundo else\n");
		p->next = aux_ptr; 


	used_mem -= aux_ptr->size;
	free_mem += aux_ptr->size;

	freep = p;

	if(total_mem == free_mem){
		drawString("Se libero con exito\n");
	}
}

/* Falta hacer:
	-syscall free (conexion userland kernel)
	-solucionar tema free que pone en 0 el size
	-hacer andar free, para probar si el malloc funciona luego de hacer frees y que no se rompa
	-hacer que malloc retorne:
	 	. tenemos que ver en que registro devuelve el valor de retorno y desde la def de la syscall
		  en userland agarrar dicho valor y tenerlo para luego llamar free si es necesario.
		. y sino lo logramos lo hacemos pasandole una nueva variable, y que el syscall_dispatcher 
		  le cargue lo que retorna malloc y asi lo agarra el usuario desde el userland con esa variable.

	-ver como mandar el 5 parametro a syscall_dispatcher, porque no funciono con rsi.
*/

