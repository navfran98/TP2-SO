#ifndef LIB_H
#define LIB_H

// aca estan los prototipos de las funciones de "lib.c" y "libasm.asm"

#include <stdint.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);
char *cpuVendor(char *result);

int strlen(char* string);
char* num_to_string(int numero);
int getNumberLength(int number);

#endif