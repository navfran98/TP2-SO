#ifndef SHELL_COMMANDS_H
#define SHELL_COMMANDS_H

#define NUMBER_OF_COMMANDS 6

void execute_command(int command, char* parameter);

static char* all_commands[] = 
{"inforeg", 
"help",
"exception0", 
"exception6",
"printmem",
"showTime", 
};

#endif