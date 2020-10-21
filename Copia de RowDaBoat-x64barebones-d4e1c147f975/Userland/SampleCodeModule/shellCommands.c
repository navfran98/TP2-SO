#include "stdio.h"
#include "shellCommands.h"
#include <string.h>
#include <syscalls.h>
#include <stdint.h>
#include "test_processes.h"
#include "test_prio.h"
#include "test_sync.h"
// #include "test_mm.h"

char * descriptions[NUMBER_OF_COMMANDS] = 
{ 
//comandos de arquitectura
"Imprime en pantalla el valor actual de todos los registros", 
"Muestra todos los distintos programas disponibles", 
"Verifica el funcionamiento de la rutina de excepcion de la division por cero", 
"Verifica el funcionamiento de la rutina de excepcion de codigo de operacion invalido", 
"Realiza un volcado de memoria de 32 bytes a partir de la direccion recibida como parametro", 
"Imprime en pantalla la hora actual", 
//comandos nuevos
"Imprime el estado de la memoria",
"Muestra el estado de todos los procesos existentes",
"Matar un proceso determinado a partir de su PID",
"Modificar la prioridad de un procesos ingresando una nueva con valor entre 1 y 10",
"Bloquear un proceso a partir de su PID",
"Muestra el estado de todos los semaforos existentes",
"Imprime el STDIN tal cual como lo recibe",
"Imprime la cantidad de lineas del input",
"Filtra las vocales del input",
"Imprime el PID junto con un saludos tras pasada una cantidad de segundos",
"Test sobre las prioridades de los procesos",
"Test sobre manejo de procesos",
"Test sobre el manejo de memoria",
"Test sobre el correcto funcionamiento de semaforos",
"Test sobre el funcionamiento sin el uso de semaforos",
};

static void inforeg();
static void help();
static void exception0();
static void exception6();
static void printmem(char* parameter);
static void showTime();

static void mem();
static void ps();
static void kill(char * s);
static void block(char * s);
static void nice(char * s, char * t);

static void sem();

extern void syscall_read(int, char*, int);
extern void syscall_write(char*, int);

void execute_command(int command, char * first_parameter, char * second_parameter) {
    switch(command){
        //Comandos de arquitectura
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
            printmem(first_parameter);
            break;
        }
        case 5:{
            showTime();
            break;
        }
        //Comandos nuevos
        case 6:{
            mem();
            break;
        }
        case 7:{
            ps();
            break;
        }
        case 8:{
            kill(first_parameter);
            break;
        }
        case 9:{
            nice(first_parameter, second_parameter);
            break;
        }
        case 10:{
            block(first_parameter);
            break;
        }
        case 11:{
            sem();
            break;
        }
        case 12:{
            //cat
            break;
        }
        case 13:{
            //wc
            break;
        }
        case 14:{
            //filter
            break;
        }
        case 15:{
            //loop
            break;
        }
        case 16:{
            test_prio();
            break;
        }
        case 17:{
            test_processes();
            break;
        }
        case 18:{
            test_mm();
            break;
        }
        case 19:{
            test_sync();
            break;
        }
        case 20:{
            test_no_sync();
            break;
        }

    }
}

//Comandos de arquitectura

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

//Comandos nuevos
static void mem(){
    syscall_check_mem_state();
}

static void ps(){
    syscall_ps();
}

static void sem(){
    print_all_semaphores();
}

static void kill(char * s){
    int i=0;
    while(s[i] != '\0'){
        //print(s[i]);
        if(!isNumber(s[i])){
            print("Invalid argument.\n");
            return;
        }
        i++;
    }

    int pid = string_to_num(s);
    if(pid < 0){
        print("Error: invalid PID\n");
        return;
    }
    if(pid <= 1){
        print("Invalid PID: IDLE and Shell can't be killed\n");
        return;
    }
    if(syscall_kill(pid) == 0){
        print("Error while trying to kill process\n");
        return;
    }
    print("Process killed successfully\n");
}

//ver si hay q recibir con getchar la nueva priority
static void nice(char * s, char * t){
    int i = 0, j=0;
    while(s[i] != '\0'){
        if(!isNumber(s[i])){
            print("Invalid first argument.\n");
            return;
        }
        i++;
    }
    while(t[j] != '\0'){
        if(!isNumber(t[j])){
            print("Invalid second argument.\n");
            return;
        }
        j++;
    }
    int priority = string_to_num(t);
    int pid = string_to_num(s);
    if(pid < 0){
        print("Error: invalid PID\n");
        return;
    }
    if(pid <= 1){
        print("Invalid PID: IDLE and Shell can't be modified\n");
        return;
    }
    // uint64_t new_priority = string_to_num(input_priority());
    if(syscall_set_priority(pid, priority) == 0){
        print("Error while trying to set a new priority\n");
        return;
    }
    print("Process priority successfully modified\n");
}

static void block(char * s){
    int i=0;
    while(s[i] != '\0'){
        //print(s[i]);
        if(!isNumber(s[i])){
            print("Invalid argument.\n");
            return;
        }
        i++;
    }
    int pid = string_to_num(s);
    if(pid < 0){
        print("Error: invalid PID\n");
        return;
    }
    if(pid == 0){
        print("Invalid PID: IDLE can't be blocked\n");
        return;
    }
    if(syscall_change_state(pid) == 0){
        print("Error while trying to change the process state\n");
        return;
    }
    print("Process state successfully modified\n");
}

// static void cat(){

// }

// static void wc(){

// }

// static void filter(){

// }

// static void loop(){

// }
