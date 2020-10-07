#include <screen_driver.h> 
#include <time.h>   /* may be implemented in the future */
#include <keyboard.h>
#include <free_list_allocator.h>
#include <buddy_allocator.h>
#include <stdint.h>

#define STD_INTPUT 0
#define STD_OUTPUT 1
#define RTC 3
#define TIMER_TICK 4    /* may be implemented in the future */
#define MALLOC_SYS 5
#define FREE_SYS 6
#define CHECK_MEM_STATE 7
#define BUDDY_MALLOC 8
#define BUDDY_FREE 9
#define CHECK_MEM_STATE_BUDDY 10
#define GET_PID 11
#define KILL_PROCESS 12
#define CREATE_PROCESS 13
#define PS 14
#define SET_PRIORITY_PROCESS 15
#define SET_STATE_PROCESS 16

//CAMBIAR NOMBRES DE LOS PARAMETROS POR NOMBRES GENERALES PARA NO CONFUNDIR
extern int segundos();
extern int minutos();
extern int horas();
//cambiar todos los tipos por uint64_t
                        //rdi         rsi                rdx            rcx          r8
void syscall_dispatcher(int ID, int file_descriptor, char * string, uint64_t size, uint64_t ptr) { 
    switch(ID){
        //estas 3 dsp van a llamar a la funcion free, malloc y state: y luego se corre la de freelist o la de buddy segun sea el caso
        case FREE_SYS:{ 
            free_list_free((void *) ptr);
            break;
        }

        case CHECK_MEM_STATE:{
            free_list_check_mem_state((uint64_t *) ptr);
            break;
        }
        
        case CHECK_MEM_STATE_BUDDY:{
            buddy_check_mem_state((uint64_t *) ptr);
            break;
        }

        case MALLOC_SYS:{ 
            return (void *) free_list_malloc(size); 
            break;
        }

        case BUDDY_FREE:{ 
            buddy_free((void*) ptr); 
            break;
        }
        
        case BUDDY_MALLOC:{ 
            return (void *) buddy_malloc(size); 
            break;
        }

        case GET_PID:{
            return get_pid();
            break;
        }

        case KILL_PROCESS:{
            return kill_process(size);
            break;
        }

        case CREATE_PROCESS:{

            break;
        }

        case PS:{
            printAll();
            break;
        }

        case SET_STATE_PROCESS:{
            return change_state(size);
            break;
        }

        case SET_PRIORITY_PROCESS:{
            return set_priority(size, ptr);
            break;
        }


        case 4:{   // WRITE syscall
            switch(file_descriptor){
                case STD_OUTPUT:{
                    drawString(string);
                    break;
                }
            }
            break;
        }
        
        case 3:{   // READ syscall
            switch(file_descriptor){
                case STD_INTPUT:{  // read from the keyboard buffer
                    string[0] = get_keyboard_buffer();
                    break;
                }
                case RTC:{
                    int seg = segundos();
					int min = minutos();
					int hora = horas();
					string[0]=hora/10 + 0x30; string[1]=hora%10 + 0x30;
					string[3]=min/10 + 0x30; string[4]=min%10 + 0x30;
					string[6]=seg/10 + 0x30; string[7]=seg%10 + 0x30;
					break;
                }
            }
            break;
        }
    }
}