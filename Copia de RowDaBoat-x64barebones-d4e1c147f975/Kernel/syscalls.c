#include <screen_driver.h> 
#include <time.h>   /* may be implemented in the future */
#include <keyboard.h>
#include <stdint.h>
#include <kernel.h>
#include <scheduler.h>
#include <interrupts.h>
#include <lib.h>
#include <memoryManager.h>


#define STD_INTPUT 0
#define STD_OUTPUT 1
#define RTC 3
#define TIMER_TICK 4    /* may be implemented in the future */
#define MALLOC 5
#define FREE 6
#define CHECK_MEM_STATE 7
#define GET_PID 11
#define KILL_PROCESS 12
#define CREATE_PROCESS 13
#define PS 14
#define SET_PRIORITY_PROCESS 15
#define SET_STATE_PROCESS 16
#define FORCE_NEW_SELECTION 17
#define HALT 18
#define BLOCK 19
#define UNBLOCK 20
#define GET_PIPE_ID 21


extern int segundos();
extern int minutos();
extern int horas();
                        //rdi         rsi           rdx          rcx          r8               r9 
void syscall_dispatcher(int ID, uint64_t arg2, char * arg3, uint64_t arg4, uint64_t arg5, uint64_t arg6) { 
    switch(ID){
        case FREE:{ 
            my_free((void *) arg5);
            break;
        }

        case CHECK_MEM_STATE:{
            my_check_mem_state();
            break;
        }

        case MALLOC:{ 
            my_malloc(arg4); 
            break;
        }


        case GET_PID:{
            get_pid();
            break;
        }

        case KILL_PROCESS:{
            kill_process(arg2);
            break;
        }

        case CREATE_PROCESS:{
            create_process((char *) arg2, (void *) arg4, arg5, arg6);
            break;
        }
        
        case HALT: {
            _hlt();
            break;
        }

        case PS:{
            print_all();
            break;
        }

        case SET_STATE_PROCESS:{
            change_state(arg2);
            break;
        }

        case SET_PRIORITY_PROCESS:{
            set_priority(arg2, arg4);
            break;
        }

        case FORCE_NEW_SELECTION: {
            force_new_selection();
            break;
        }

        case BLOCK: {
            block(arg2);
            break;
        }

        case UNBLOCK: {
            unblock(arg2);
            break;
        }

        case GET_PIPE_ID:{
            get_pipe_id();
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