#ifndef STDIO_H
#define STDIO_H

#include <stdint.h>

void print(char* string);
char getChar(void);
void putchar(char c);
void print_pipe(char* string, uint64_t pipe_id);
void read_pipe(uint64_t pipe_id, char * resp);

#endif