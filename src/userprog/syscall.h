#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

void syscall_init (void);

/* System Calls Definetions*/
void halt (void* vArgs);
void exit (void* vArgs);
void exec (void* vArgs);
void wait (void* vArgs);
void create (void* vArgs);
void remove (void* vArgs);
void open (void* vArgs);
void filesize (void* vArgs);
void read (void* vArgs);
void write (void* vArgs);
void seek (void* vArgs);
void tell (void* vArgs);
void close (void* vArgs);

#endif /* userprog/syscall.h */
