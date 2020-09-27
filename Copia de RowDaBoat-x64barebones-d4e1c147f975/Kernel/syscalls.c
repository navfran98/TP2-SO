#include <screen_driver.h> 
#include <time.h>   /* may be implemented in the future */
#include <keyboard.h>
#include <mm.h>
#include <stdint.h>

#define STD_INTPUT 0
#define STD_OUTPUT 1
#define RTC 3
#define TIMER_TICK 4    /* may be implemented in the future */
#define MALLOC_SYS 5
#define FREE_SYS 6
#define CHECK_MEM_STATE 7

extern int segundos();
extern int minutos();
extern int horas();
//cambiar todos los tipos por uint64_t
                        //rdi         rsi                rdx            rcx          r8
void syscall_dispatcher(int ID, int file_descriptor, char * string, uint8_t size, uint64_t ptr) { 
    switch(ID){

        case FREE_SYS:{ 
            my_free((void *) ptr);
            break;
        }

        case CHECK_MEM_STATE:{
            check_mem_state((unsigned *) ptr);
            break;
        }

        case MALLOC_SYS:{ //MALLOC syscall
           return (void *) my_malloc(size); 
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