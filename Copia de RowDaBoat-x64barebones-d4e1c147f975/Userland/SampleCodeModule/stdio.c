//#include "lib.h"
#include "string.h"

#define STD_INPUT 0

void print(char* string) {
  syscall_write(string, strlen(string));
}

void putchar(char c) {
  char to_write[] = {'\0', '\0'};
  to_write[0] = c;
  print(to_write);
}

char getChar() {
  char resp;
  syscall_read(STD_INPUT, &resp, 1);
  return resp;
}

