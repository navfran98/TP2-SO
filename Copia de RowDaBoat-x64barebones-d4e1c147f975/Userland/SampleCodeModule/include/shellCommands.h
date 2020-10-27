#ifndef SHELL_COMMANDS_H
#define SHELL_COMMANDS_H

#define NUMBER_OF_COMMANDS 22

void execute_command(int cmds[2], char * first_parameter, char * second_parameter, int n, int cmds_ground[2]);

static char* all_commands[] = 
{//comandos de arquitectura
"inforeg", 
"help",
"exception0", 
"exception6",
"printmem",
"showTime", 
//comandos nuevos
"mem",
"ps",
"kill",
"nice",
"block",
"sem",
"cat",
"wc",
"filter",
"loop",
"pipes",
"test_prio",
"test_processes",
"test_mm",
"test_sync",
"test_no_sync"
};

#endif