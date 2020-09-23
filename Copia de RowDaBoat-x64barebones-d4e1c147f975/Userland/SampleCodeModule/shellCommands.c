#include "stdio.h"
#include "shellCommands.h"

char * descriptions[NUMBER_OF_COMMANDS] = 
{ 
"Imprime en pantalla el valor actual de todos los registros", 
"Muestra todos los distintos programas disponibles", 
"Verifica el funcionamiento de la rutina de excepcion de la division por cero", 
"Verifica el funcionamiento de la rutina de excepcion de codigo de operacion invalido", 
"Realiza un volcado de memoria de 32 bytes a partir de la direccion recibida como parametro", 
"Imprime en pantalla la hora actual", 
};

static void inforeg();
static void help();
static void exception0();
static void exception6();
static void printmem(char* parameter);
static void showTime();

extern void syscall_read(int, char*, int);
extern void syscall_write(char*, int);

void execute_command(int command, char* parameter) {
    switch(command){
        case 0:{
            inforeg();
            break;
        }
        case 1:{
            help();
            break;
        }
        case 2:{
            exception0();
            break;
        }
        case 3/*****************/:{
            exception6();
            break;
        }
        case 4:{
            printmem(parameter);
            break;
        }
        case 5:{
            showTime();
            break;
        }
    }
}

static void inforeg(){
    /* Arqui Legacy */
    print("inforeg");
}

static void help() {
    /* NO OLVIDARSE DE ACTUALIZARLO */
    for(int i=0; i<NUMBER_OF_COMMANDS; i++) {
        print(all_commands[i]);
        print(":  ");
        print(descriptions[i]);
        print("\n");
    }
}

static void exception0() {
    /* Arqui Legacy */
    print("exception0");
}

static void exception6() {
    /* Arqui Legacy */
    print("exception6");
}

static void printmem(char* parameter) {
    /* Arqui Legacy */
    print("printmem");
}

static void showTime() {
    char time[]= {-1, -1, ':', -1, -1, ':', -1, -1, '\0'};
    syscall_read(3, time, 1);
    print(time);
    print(" UTC");
}