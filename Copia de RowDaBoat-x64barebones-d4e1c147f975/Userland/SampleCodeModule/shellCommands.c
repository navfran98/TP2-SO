#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <syscalls.h>
#include <shellCommands.h>
#include <test_processes.h>
#include <test_prio.h>
#include <test_sync.h>
#include <test_mm.h>
#include <pipes_manager.h>
#include <semaphores_manager.h>

#define FOREGROUND 0
#define BACKGROUND 1

static char * descriptions[NUMBER_OF_COMMANDS] = 
{ 
"Imprime en pantalla el valor actual de todos los registros", 
"Muestra todos los distintos programas disponibles", 
"Verifica el funcionamiento de la rutina de excepcion de la division por cero", 
"Verifica el funcionamiento de la rutina de excepcion de codigo de operacion invalido", 
"Realiza un volcado de memoria de 32 bytes a partir de la direccion recibida como parametro", 
"Imprime en pantalla la hora actual", 
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
"Imprime la lista de todos los pipes con sus propiedades",
"Test sobre las prioridades de los procesos",
"Test sobre manejo de procesos",
"Test sobre el manejo de memoria",
"Test sobre el correcto funcionamiento de semaforos",
"Test sobre el funcionamiento sin el uso de semaforos"
};

static void inforeg();
static void help();
static void exception0();
static void exception6();
static void printmem(char* parameter);
static void showTime();

static void mem();
static void ps();
static void list_sem();
static void kill(char * s);
static void block(char * s);
static void nice(char * s, char * t);
static void filter();
static void cat();
static void wc();
static void loop();
static void list_pipes();

int is_vocal(char c);
extern char* num_to_string(int num);
void create_possible_piped_process(char * pname, void * ptr, pipe * p, uint64_t ground, int n, int i);

void execute_command(int cmds[2], char * first_parameter, char * second_parameter, int n, int cmds_ground[2]) {
    int commands[2] = {0,0};
    commands[0] = cmds[0];
    commands[1] = cmds[1];
    pipe* p;
    if(n > 1){
        p = create_pipe();
    }
    // uint64_t aux_pid;
    for(uint8_t i = 0; i < n; i++){
        switch(commands[i]){
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
            case 3:{
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
                list_sem();
                break;
            }
            case 12:{
                create_possible_piped_process("CAT", &cat, p, cmds_ground[i], n, i);
                break;
            }
            case 13:{
                create_possible_piped_process("WC", &wc, p, cmds_ground[i], n, i);
                break;
            }
            case 14:{
                create_possible_piped_process("FILTER", &filter, p, cmds_ground[i], n, i);
     
                break;
            }
            case 15:{
                create_possible_piped_process("LOOP", &loop, p, cmds_ground[i], n, i);
          
                break;
            }
            case 16:{
                create_possible_piped_process("PIPE", &list_pipes, p, cmds_ground[i], n, i);
                break;
            }
            case 17:{
                test_prio();
                break;
            }
            case 18:{
                test_processes();
                break;
            }
            case 19:{
                test_mm();
                break;
            }
            case 20:{
                test_sync();
                break;
            }
            case 21:{
                test_no_sync();
                break;
            }
        }
    }   
}

//Comandos de arquitectura

static void inforeg(){
    print("inforeg");
}

static void help() {
    for(int i=0; i<NUMBER_OF_COMMANDS; i++) {
        print(all_commands[i]);
        print(":  ");
        print(descriptions[i]);
        print("\n");
    }
}

static void exception0() {
    print("exception0");
}

static void exception6() {
    print("exception6");
}

static void printmem(char* parameter) {
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

static void list_sem(){
    print_all_semaphores();
}

static void kill(char * s){
    int i=0;
    while(s[i] != '\0'){
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

static void cat(){
    uint64_t id = syscall_get_pipe_id();
    print_pipe("COMANDO CAT\nCOMANDO CAT\nCOMANDO CAT\nCOMANDO CAT\n", id);
    unblock_pipe_partner(id);
    syscall_kill(syscall_get_pid());
}

static void wc(){
    char * s = read_pipe(syscall_get_pipe_id());
    int lines = 0;
    for(int i=0; s[i] != 0; i++){
        if(s[i] == '\n'){
            lines++;
        }
    }
    print("El numero de lineas del input es: ");
    print(num_to_string(lines));
    print("\n");
    unblock_pipe_partner(syscall_get_pipe_id());
    syscall_kill(syscall_get_pid());
}

static void filter(){
    char * s = read_pipe(syscall_get_pipe_id());
    print(s);
    int j = 0;
    for(int i = 0; s[i] != '\0'; i++){
        if(!is_vocal(s[i])){
            putchar(s[i]);
        }
    }
    unblock_pipe_partner(syscall_get_pipe_id());
    syscall_kill(syscall_get_pid());
}

static void loop(){
    int i = 0;
    int j = 0;
    while(1){
        //busy waiting
        if(j == 10000000){
            print("HOLA! SOY EL PROCESO: ");
            print(num_to_string(syscall_get_pid()));
            print(" ");
            j=0;
        }
        j++;
        i++;
    }
}

static void list_pipes(){
    print_all_pipes();
    syscall_kill(syscall_get_pid());
}

void create_possible_piped_process(char * pname, void * ptr, pipe * p, uint64_t ground, int n, int i){
    if(n > 1){
        uint64_t aux_pid = syscall_create_process(pname, ptr, p->pipe_id, ground);
        if(aux_pid != -1){
            if(i == 0){
                p->pid1 = aux_pid;
            }else{
                syscall_block(aux_pid);
                p->pid2 = aux_pid;
            }
        }
    }else{
        syscall_create_process(pname, ptr, 0, ground);
    }
}

int is_vocal(char c){
    if(c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' || c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U'){
        return 1;
    }
    return 0;
}