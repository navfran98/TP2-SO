#include "stdio.h"
#include "shellCommands.h"
#include "string.h"
#include <stdint.h>
#include "syscalls.h"
#include "test_processes.h"
#include "test_prio.h"
#include "test_sync.h"

#define SHELL_BUFFER_SIZE (128)+(128/2)  // 1 linea y media

static void shell_main();
static int search_command(char command_with_possible_parameter[], char * first_parameter, char * second_parameter);

static char shell_buffer[SHELL_BUFFER_SIZE] = {'\0'};
static uint8_t k = 0; // to move across the buffer

void nava(){
    print("NAVA EJECUTANDO\n");
    syscall_ps();
    syscall_kill(syscall_get_pid());
    // while(1){
    //     syscall_halt();
    // }
}

void start_shell() {

    print(">>");
    //test_mm();

    //test_mm();
    //test_processes();
    // test_prio();
    // test_no_sync();
    //funciona poniendo 1=BACK y 0=FORE
    //uint64_t a = syscall_get_pid();
    // int i=0;
    // while(i<5){
        // test_no_sync();
    // syscall_create_process("NAVA", &nava, 3, 0);
    // syscall_create_process("NAVA2", &nava, 3, 0);
        // syscall_create_process("Shell", &shell_main, 5,0);
    //     i++;
    // }
    //CON ESTO SE VE Q ITERA BIEN SALTEANDOSE AL IDLE
    //PROBLEMAS AL LLAMAR CALL_SCHEDULER ----->SOLUCIONAR IMPORTANTE
    // syscall_ps();
    // syscall_kill(a);
    // syscall_ps();
    // syscall_change_state(3);
    // syscall_force_new_selection();
    // syscall_force_new_selection();
    // syscall_ps();

    // uint64_t new_prio = 4;
    // syscall_set_priority(a, new_prio); //pasando por ejemplo (a, 2) te pone cualquier cosa al caster el 2
    // syscall_ps();
    
    shell_main();
}


static void shell_main() {
    print("SHELL MAIN\n");
    char c;
    int command;
    while(1) {
        c = getChar();
        if(c != -1) {
            if(c == '\n') {
                putchar(c);     // print the '\n' to move to the next line
                shell_buffer[k++] = '\0';
                char * first_parameter = "X";
                char * second_parameter = "Y";
                command = search_command(shell_buffer, first_parameter, second_parameter);
                if(command != -1)
                    execute_command(command, first_parameter, second_parameter);
                else
                    print("   Command not found");
                putchar(c);  // print the '\n' to move to the next line
                // empty our shell_buffer to prepare for the next line
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

static int search_command(char command_with_possible_parameter[], char * first_parameter, char * second_parameter) {
    
    int number_of_command = -1;
    char command_without_parameter[SHELL_BUFFER_SIZE] = {'\0'};

    uint8_t i=0;
    for( ; command_with_possible_parameter[i]!='\0' && command_with_possible_parameter[i]!=' '; i++){
        command_without_parameter[i] = command_with_possible_parameter[i];
    }
    command_without_parameter[i] = '\0';
    uint8_t command_found = 0;
    for(uint8_t j=0; j<NUMBER_OF_COMMANDS && !command_found; j++) {
        if( strcmp(command_without_parameter, all_commands[j]) == 0) {
            number_of_command = j;
            command_found = 1;
        }
    }

    if(number_of_command == -1) {
        // si no encontre el comando, ni me fijo en el parametro, y salgo de esta funcion
        return -1;
    }


    if(command_with_possible_parameter[i] == ' ') {
        // there is also a parameter next to the command
        uint8_t j2=0,j3=0;
        i++;
        for(i; command_with_possible_parameter[i]!='\0' && command_with_possible_parameter[i] != ' ';  i++) {
            first_parameter[j2++] = command_with_possible_parameter[i];
        }
        first_parameter[j2++] = '\0';
     
        if(command_with_possible_parameter[i] == ' '){
            i++; //space encontrado
            for(i; command_with_possible_parameter[i]!='\0'; i++) {
                second_parameter[j3++] = command_with_possible_parameter[i];
            }
            second_parameter[j3++] = '\0';
        }
  
    }

    return number_of_command;
}
