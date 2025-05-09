#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

void syscall_init (void);

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
