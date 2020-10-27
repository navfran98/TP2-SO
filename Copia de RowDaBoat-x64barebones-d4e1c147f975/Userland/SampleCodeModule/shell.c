#include <stdio.h>
#include <shellCommands.h>
#include <string.h>
#include <stdint.h>
#include <syscalls.h>

#define SHELL_BUFFER_SIZE (128)+(128/2)  // 1 linea y media
#define FOREGROUND 0
#define BACKGROUND 1
extern char* num_to_string(int num);
static void shell_main();
static void search_command(char buffer[], char * first_parameter, char * second_parameter, int cmds_ground[2], int cmds[2]);
static char shell_buffer[SHELL_BUFFER_SIZE] = {'\0'};
static uint8_t k = 0; // to move across the buffer


void start_shell() {
    print(">>");    
    shell_main();
}

static void shell_main() {
    char c;
    while(1) {
        c = getChar();
        if(c != -1) {
            if(c == '\n') {
                putchar(c);     // print the '\n' to move to the next line
                shell_buffer[k++] = '\0';
                int cmds[2] = {-1, -1};
                int cmds_ground[2] = {FOREGROUND, FOREGROUND};
                char * first_parameter = "X";
                char * second_parameter = "Y";
                search_command(shell_buffer, first_parameter, second_parameter, cmds_ground, cmds);
                if(cmds[0] != -1){
                    if(cmds[1] == -1){
                        execute_command(cmds, first_parameter, second_parameter, 1, cmds_ground);
                    }else{
                        execute_command(cmds, first_parameter, second_parameter, 2, cmds_ground);                        
                    }
                }
                else
                    print("Command not found");
                putchar(c);
                for(int i=0; shell_buffer[i] != '\0'; i++) {
                    shell_buffer[i] = '\0';
                }
                k = 0;
                print(">>");
            }
            else if(c=='\b' && k>0) {
                shell_buffer[k--] = '\0';
                putchar(c);
            }
            else { // it is a normal character
                if(k < SHELL_BUFFER_SIZE) {
                    shell_buffer[k++] = c;
                    putchar(c);
                }
            }
        }
    }
}

//HACER QUE CUANDO RECIBE UN '&' ALFINAL SIGINIFICA QUE LO CREA EN BACKGROUND
//PERMITIR QUE EJECUTAR DOS PROCESOS USANDO '|'. Ejemplo: p1 | p2
//VER DE GUARDAR EL DONDE ESCRIBIR Y DONDE LEER PARA LOS PROCESOS CAT, FILTER, WC (NACHO CREA UN FG TMB)

int get_command_number(char * s, uint8_t * n){
    uint8_t i = *n;
    uint8_t k = 0;
    int number_of_command = -1;
    char aux[SHELL_BUFFER_SIZE] = {'\0'};
    for(; s[i] != '\0' && s[i] != ' ' && s[i] != '&'; i++){
        aux[k++] = s[i];

    }
    aux[k] = '\0';
    uint8_t command_found = 0;
    for(uint8_t j = 0; j < NUMBER_OF_COMMANDS && !command_found; j++) {
        if( strcmp(aux, all_commands[j]) == 0) {
            number_of_command = j;
            command_found = 1;
        }
    }
    *n = i;
    return number_of_command;    
}

static void search_command(char buffer[], char * first_parameter, char * second_parameter, int cmds_ground[2], int cmds[2]) {
    uint8_t i = 0;
    //buscamos el numero del comando a ejecutar
    cmds[0] = get_command_number(buffer, &i);
    if(cmds[0] == -1) {
        return; 
    }

    if(buffer[i] == '&'){
        cmds_ground[0] = BACKGROUND;
        return;
    }
    
    if(buffer[i] == ' '){
        i++;
        // there is also a parameter next to the command
        if(buffer[i] != '|') {
            uint8_t j2 = 0, j3 = 0;
            for(; buffer[i] != '\0' && buffer[i] != ' ' && buffer[i] != '&';  i++) {
                first_parameter[j2++] = buffer[i];
            }
            first_parameter[j2++] = '\0';

            if(buffer[i] == '&'){
                cmds_ground[0] = BACKGROUND;
                return;
            }
            
            if(buffer[i] == ' '){
                i++; //space encontrado
                for(; buffer[i]!='\0' && buffer[i] != '&'; i++) {
                    second_parameter[j3++] = buffer[i];
                }
                second_parameter[j3++] = '\0';
            }

            if(buffer[i] == '&'){
                //prender flag bf
                cmds_ground[0] = BACKGROUND;
                return;
            }
        } else {
           
            i += 2;
            cmds[1] = get_command_number(buffer, &i);
            if(cmds[1] == -1){
                cmds[0] = -1;
            }
            return;
        }
    }
    return;
}

