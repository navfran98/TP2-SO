#ifndef STRING_H
#define STRING_H

#include <stdint.h>


int strcmp (const char *p1, const char *p2);

int strlen(char* string);

int string_to_num(char* parameter);

int isNumber(char c);

void * memset(void * destiation, uint32_t c, uint64_t length);

#endif