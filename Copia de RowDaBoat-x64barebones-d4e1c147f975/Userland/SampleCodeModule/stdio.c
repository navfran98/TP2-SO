// This is a personal academic project. Dear PVS-Studio, please check it.
#include <stdint.h>
#include <string.h>
#include <syscalls.h>
#include <stdio.h>
#include <pipes_manager.h>
#define KEY_CHAR '$'
#define STD_INPUT 0

void print(char* string){
  syscall_write(string,strlen(string));
}

char getChar() {
  char resp;
  syscall_read(STD_INPUT, &resp, 1);
  return resp;
}

void putchar(char c) {
  char to_write[] = {'\0', '\0'};
  to_write[0] = c;
  print(to_write);
}

void print_pipe(char * s, uint64_t pipe_id) {
  if(pipe_id != 0){
    pipe_write(pipe_id, s, strlen(s));
  }else{
    print(s);
  }
}

void read_pipe(uint64_t pipe_id, char * resp){
  // char * resp;
  pipe_read(pipe_id, resp);
  // return resp;
}

