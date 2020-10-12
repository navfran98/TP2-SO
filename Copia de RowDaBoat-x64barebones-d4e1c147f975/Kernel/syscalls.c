#include <screen_driver.h> 
#include <time.h>   /* may be implemented in the future */
#include <keyboard.h>
#include <free_list_allocator.h>
#include <buddy_allocator.h>
#include <stdint.h>
#include <kernel.h>
#include <scheduler.h>

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
#define FORCE_NEW_SELECTION 17

//CAMBIAR NOMBRES DE LOS PARAMETROS POR NOMBRES GENERALES PARA NO CONFUNDIR
extern int segundos();
extern int minutos();
extern int horas();
//cambiar todos los tipos por uint64_t
            //rdi         rsi                rdx            rcx          r8    r9
void syscall_dispatcher(int ID, int arg2, char * arg3, uint64_t arg4, uint64_t arg5, uint64_t arg6) { 
    switch(ID){
        //estas 3 dsp van a llamar a la funcion free, malloc y state: y luego se corre la de freelist o la de buddy segun sea el caso
        case FREE_SYS:{ 
            free_list_free((void *) arg5);
            break;
        }

        case CHECK_MEM_STATE:{
            free_list_check_mem_state((uint64_t *) arg5);
            break;
        }
        
        case CHECK_MEM_STATE_BUDDY:{
            buddy_check_mem_state((uint64_t *) arg5);
            break;
        }

        case MALLOC_SYS:{ 
            return (void *) free_list_malloc(arg4); 
            break;
        }

        case BUDDY_FREE:{ 
            buddy_free((void*) arg5); 
            break;
        }
        
        case BUDDY_MALLOC:{ 
            return (void *) buddy_malloc(arg4); 
            break;
        }

        case GET_PID:{
            return get_pid();
            break;
        }

        case KILL_PROCESS:{
            return kill_process(arg4);
            break;
        }

        case CREATE_PROCESS:{
            /*name, priority, contexto*/
            // if(name == "SHELL"){
            //     void * rip = shellModuleAddress;
            // }
            //                 //name , , prio, cont
            // create_process(arg2, rip, arg4 , arg5)
            break;
        }

        case PS:{
            print_all();
            break;
        }

        case SET_STATE_PROCESS:{
            return change_state(arg4);
            break;
        }

        case SET_PRIORITY_PROCESS:{
            return set_priority(arg4, arg5);
            break;
        }

        case FORCE_NEW_SELECTION: {
            force_new_selection();
            break;
        }

        case 4:{   // WRITE syscall
            switch(arg2){
                case STD_OUTPUT:{
                    drawString(arg3);
                    break;
                }
            }
            break;
        }
        
        case 3:{   // READ syscall
            switch(arg2){
                case STD_INTPUT:{  // read from the keyboard buffer
                    arg3[0] = get_keyboard_buffer();
                    break;
                }
                case RTC:{
                    int seg = segundos();
					int min = minutos();
					int hora = horas();
					arg3[0]=hora/10 + 0x30; arg3[1]=hora%10 + 0x30;
					arg3[3]=min/10 + 0x30; arg3[4]=min%10 + 0x30;
					arg3[6]=seg/10 + 0x30; arg3[7]=seg%10 + 0x30;
					break;
                }
            }
            break;
        }
    }
}