#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H
#include "threads/thread.h"

void syscall_init (void);

/* 
 * get eax to pass the return value
 * cast arg1 to the type you want in the system call handler   
*/
typedef struct { uint32_t *eax; int arg1; int arg2; int arg3; } Arguments;

#define ERROR -1
#define ERROR_ARG ((Arguments[]){ { .arg1 = -1 } })
#define SAFE(ptr) convert(validate(ptr))
#define STDIN_FILENO 0
#define STDOUT_FILENO 1

/* System Calls Definetions*/
void halt (Arguments *args);
void exit (Arguments *args);
void exec (Arguments *args);
void wait (Arguments *args);
void create (Arguments *args);
void remove (Arguments *args);
void open (Arguments *args);
void filesize (Arguments *args);
void read (Arguments *args);
void write (Arguments *args);
void seek (Arguments *args);
void tell (Arguments *args);
void close (Arguments *args);

#endif /* userprog/syscall.h */
