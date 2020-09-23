#include <keyboard.h>
#include <stdint.h>

extern int getKey(void);
static char get_letter_code(void);
static uint8_t isLetra(uint8_t n);

// estos 2 arrays estaban en keyboard.h, pero me tiraban warnings raros. Entonces los puse aca y listo
static char map[] = { '\0', '\0'/*esc*/, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
                    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
                    '\0'/*Lctrl*/, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', '\0'/*lshift*/,
                    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', '\0'/*rshift*/, '*'/*keypad * */, '\0'/*lalt*/, ' ',
                    '\0'/*capslock*/, '\0'/*f1*/, '\0'/*f2*/, '\0'/*f3*/, '\0'/*f4*/, '\0'/*f5*/, '\0'/*f6*/, '\0'/*f7*/, '\0'/*f8*/, '\0'/*f9*/, '\0'/*f10*/,
                    '\0'/*numlock*/, '\0'/*scrollLock*/, '7'/*keypad*/, '8'/*keypad*/, '9'/*keypad*/, '-'/*keypad*/, '4'/*keypad*/, 
                    '5'/*keypad*/, '6'/*keypad*/, '+'/*keypad*/, '1'/*keypad*/, '2'/*keypad*/, '3'/*keypad*/, '0'/*keypad*/, '.'/*keypad*/, '\0','\0','\0',
                    '\0'/*f11*/, '\0'/*f12*/
};
static char map_shift[] = {'\0', '\0'/*esc*/, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
                    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
                    '\0'/*Lctrl*/, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', '\0'/*lshift*/,
                    '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', '\0'/*rshift*/, '*'/*keypad * */, '\0'/*lalt*/, ' ',
                    '\0'/*capslock*/, '\0'/*f1*/, '\0'/*f2*/, '\0'/*f3*/, '\0'/*f4*/, '\0'/*f5*/, '\0'/*f6*/, '\0'/*f7*/, '\0'/*f8*/, '\0'/*f9*/, '\0'/*f10*/,
                    '\0'/*numlock*/, '\0'/*scrollLock*/, '7'/*keypad*/, '8'/*keypad*/, '9'/*keypad*/, '-'/*keypad*/, '4'/*keypad*/, 
                    '5'/*keypad*/, '6'/*keypad*/, '+'/*keypad*/, '1'/*keypad*/, '2'/*keypad*/, '3'/*keypad*/, '0'/*keypad*/, '.'/*keypad*/, '\0','\0','\0',
                    '\0'/*f11*/, '\0'/*f12*/
};

// used to store only the last MAKE CODE pushed
static char buffer = -1;

void keyboard_handler(void){
    char letra = get_letter_code();
    if(letra == -1) {
        // no añado nada al buffer ya que el teclado no me dio ninguna tecla nueva
        return;
    }
    buffer = letra;  // añado la letra al buffer
}

char get_keyboard_buffer(void) {
    char aux = buffer;
    buffer = -1;
    return aux;
}

char caps_lock = 0;
char shift_pressed = 0;
char ctrl_pressed = 0;
char alt_pressed = 0;
char esc_pressed = 0;

// Si es ctrl,shift, etc prendo el flag, y retorno -1. Sino retorno el caracter correspondiente
static char get_letter_code(void){
    uint8_t code = getKey(); //aca obtengo el codigo de la letra
    if(code == 0) {
        // el teclado no tenia ninguna tecla para darme
        return -1;
    }
    /*if(code>=0x81 && code!=0xAA && code!=0XB6) {
        return -1;
    }*/

    /*while(code>=81) {
        code = getKey();
    }*/
    uint8_t len = sizeof(map);
    switch(code){
        case L_SHIFT:
        case R_SHIFT:
                shift_pressed = 1;return -1;            
        case L_SHIFT_RELEASED:
        case R_SHIFT_RELEASED:
                shift_pressed = 0;return -1;

        case L_CTRL:
                ctrl_pressed = 1;return -1;
        case L_CTRL_RELEASED:
                ctrl_pressed = 0;return -1;

        case L_ALT:
                alt_pressed = 1;return -1;
        case L_ALT_RELEASED:
                alt_pressed = 0;return -1;

        case CAPSLOCK:
                if(caps_lock == 0)
                    caps_lock = 1;
                else
                    caps_lock = 0;
                return -1;
        case ESC:
                esc_pressed = 1;return -1;
        case ESC_RELEASED:
                esc_pressed = 0;return -1;

        case DOWN:
        case UP:
        case LEFT:
        case RIGHT:
        case TAB:
            return -1;

        //Si no es ninguna tecla especial, entonces debe ser una letra/numero/signo
        default:
            if (code > 0  &&  code < len  &&  map[code]!='\0'){
                if(shift_pressed == 0){
                    if(caps_lock == 0){
                        //Busco en la tabla en minuscula:
                        return map[code];
                    }
                    else if(caps_lock == 1){
                        //Busco en la tabla en MAYUSCULA
                        if(isLetra(code))
                            return map_shift[code];
                        return map[code];
                    }
                }
                if(shift_pressed == 1){
                    if(caps_lock == 0){
                        //Busco en la tabla en MAYUSCULA
                        return map_shift[code];
                    }
                    else if(caps_lock == 1){
                        //Busco en la tabla en minuscula
                        if(isLetra(code))
                            return map[code];
                        return map_shift[code];
                    }
                }       
            }
            else{
                return -1;
            }
            break;
    }
    return -1;
}

static uint8_t isLetra(uint8_t n){
    if(n >= 0x10 && n <=0x19)
        return 1;
    if(n >= 0x1E  && n <= 0x26)
        return 1;
    if(n >= 0x2C &&  n <= 0x32)
        return 1;
    return 0;
}