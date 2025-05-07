#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"

#define ERROR -1

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

typedef void (*SystemCall)(void*);

/* 
 * get eax to pass the return value
 * cast arg1 to the type you want in the system call handler   
*/
typedef struct { uint32_t *eax; void* arg1; void* arg2; void* arg3; } Arguments;

SystemCall systemCalls[] = {
  halt,
  exit,
  exec,
  wait,
  create,
  remove,
  open,
  filesize,
  read,
  write,
  seek,
  tell,
  close
};

void halt (void* vArgs){
  shutdown_power_off();
}

void exit (void* vArgs){
  Arguments* args = (Arguments*)vArgs;
  int status = *(int*) args->arg1;
  //implement
}

void exec (void* vArgs){
  Arguments* args = (Arguments*)vArgs;
  tid_t pid = *(tid_t*) args->arg1;
  //implement
}

void wait (void* vArgs){
  Arguments* args = (Arguments*)vArgs;
  const char *cmd_line = (char*) args->arg1; 
  //implement
}

void create (void* vArgs){
  Arguments* args = (Arguments*)vArgs;

  const char *file = (char*) args->arg1;
  unsigned initial_size = *(unsigned*) args->arg2;
  //implement
}

void remove (void* vArgs){
  Arguments* args = (Arguments*)vArgs;
  const char *file = (char*) args->arg1;
  //implement
}

void open (void* vArgs){
  Arguments* args = (Arguments*)vArgs;
  const char *filename = (char*) args->arg1;
  //implement
}

void filesize (void* vArgs){
  Arguments* args = (Arguments*)vArgs;
  int fd = *(int*) args->arg1;
  //implement
}

void read (void* vArgs){
  Arguments* args = (Arguments*)vArgs;
  int fd = *(int*) args->arg1;
  void *buffer = args->arg2;
  unsigned size = *(unsigned*) args->arg3;
  //implement
}

void write (void* vArgs){
  Arguments* args = (Arguments*)vArgs;
  int fd = *(int*) args->arg1;
  const void *buffer = args->arg2;
  unsigned size = *(unsigned*) args->arg3;
  //implement
}

void seek (void* vArgs){
  Arguments* args = (Arguments*)vArgs;
  int fd = *(int *) args->arg1;
  unsigned position = *(unsigned *) args->arg2;
  //implement
}

void tell (void* vArgs){
  Arguments* args = (Arguments*)vArgs;
  int fd = *(int *) args->arg1;
  //implement
}

void close (void* vArgs){
  Arguments* args = (Arguments*)vArgs;
  int fd = *(int *) args->arg1;
  // implement
}

void * 
validate(void *address){
  if(address < (void *) 0x08048000 || address >= PHYS_BASE){
    exit(ERROR);
  }
  return address;
}

void *
convert(void *address){
  void *vaddress = pagedir_get_page(thread_current()->pagedir, address);
  if(vaddress == NULL){
    exit(ERROR);
  }
  return vaddress;
}

void * load_args(void *esp, uint32_t *eax){
  Arguments *Args = malloc(sizeof(Arguments));
  Args->arg1 = convert(validate(esp));
  Args->arg2 = convert(validate((void *)((int *)esp + 1)));
  Args->arg3 = convert(validate((void *)((int *)esp + 2)));
  Args->eax = eax;
  return (void *) Args;
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  int call_code = *(int *)f->esp;
  void* args = load_args(f->esp + 1, &f->eax);
  systemCalls[call_code](args);
}