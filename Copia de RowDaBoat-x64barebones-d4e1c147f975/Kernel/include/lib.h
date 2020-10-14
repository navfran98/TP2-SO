#ifndef LIB_H
#define LIB_H

// aca estan los prototipos de las funciones de "lib.c" y "libasm.asm"

#include <stdint.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);
int strcmp (const char *p1, const char *p2);
char *cpuVendor(char *result);

int strlen(char* string);
char* num_to_string(int numero);
int getNumberLength(int number);
void call_scheduler();
uint64_t init_stack(void * ptr, uint64_t rip);

#endif