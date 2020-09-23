#include <screen_driver.h>
#include <time.h>   /* may be implemented in the future */
#include <keyboard.h>

#define STD_INTPUT 0
#define STD_OUTPUT 1
#define RTC 3
#define TIMER_TICK 4    /* may be implemented in the future */

extern int segundos();
extern int minutos();
extern int horas();

void syscall_dispatcher(int ID, int file_descriptor, char* string, int length/*NEVER USED*/  ) {
    switch(ID){
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