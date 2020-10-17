#ifndef STRING_H
#define STRING_H

#include <syscalls.h>

int strcmp (const char *p1, const char *p2);

int strlen(char* string);

int string_to_num(char* parameter);

void * memset(void * destiation, int32_t c, uint64_t length);

#endif