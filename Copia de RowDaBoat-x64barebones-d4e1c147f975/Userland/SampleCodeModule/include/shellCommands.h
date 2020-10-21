#include <test_prio.h>
#include <test_processes.h>
#include <test_util.h>
#include <test_sync.h>

#ifndef SHELL_COMMANDS_H
#define SHELL_COMMANDS_H

#define NUMBER_OF_COMMANDS 21

void execute_command(int command, char * first_parameter, char * second_parameter);

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
"test_prio",
"test_processes",
"test_mm",
"test_sync",
"test_no_sync",
};

#endif