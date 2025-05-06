#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"

#define ERROR -1

static void syscall_handler (struct intr_frame *);

uint32_t *eax;

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

typedef void (*SystemCall)(void*);

typedef struct { void* arg1; } OneArg;
typedef struct { void* arg1; void* arg2; } TwoArg;
typedef struct { void* arg1; void* arg2; void* arg3; } ThreeArg; 

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

// OneArg one_arg;
// TwoArg two_arg;
// ThreeArg three_arg;

// void *args[] = {
  
// };

void halt (void* vArgs){
  shutdown_power_off();
}

void exit (void* vArgs){
  OneArg* Args = (OneArg*) vArgs;
  int status = *(int*) Args->arg1;
  //implement
}

void exec (void* vArgs){
  OneArg* Args = (OneArg*) vArgs;
  tid_t pid = *(tid_t*) Args->arg1; 
  process_execute(pid);
  //implement
}

void wait (void* vArgs){
  OneArg* Args = (OneArg*) vArgs;
  const char *cmd_line = (char*)Args->arg1;

  tid_t tid = process_wait(cmd_line); 
  *eax = tid;
  //implement
}

void create (void* vArgs){
  TwoArg* Args = (TwoArg*) vArgs;
  const char *file = (char*)Args->arg1;
  unsigned initial_size = *(unsigned*)Args->arg2;
  //implement
}

void remove (void* vArgs){
  OneArg* Args = (OneArg*) vArgs;
  const char *file = (char*)Args->arg1;
  //implement
}

void open (void* vArgs){
  OneArg* Args = (OneArg*) vArgs;
  const char *file = (char*)Args->arg1;
  //implement
}

void filesize (void* vArgs){
  OneArg* Args = (OneArg*) vArgs;
  int fd = *(int*)Args->arg1;
  //implement
}

void read (void* vArgs){
  ThreeArg* Args = (ThreeArg*) vArgs;
  int fd = *(int*)Args->arg1;
  void *buffer = Args->arg2;
  unsigned size = *(unsigned*)Args->arg3;
  //implement
}

void write (void* vArgs){
  ThreeArg* Args = (ThreeArg*) vArgs;
  int fd = *(int*)Args->arg1;
  const void *buffer = Args->arg2;
  unsigned size = *(unsigned*)Args->arg3;
  //implement
}

void seek (void* vArgs){
  TwoArg* Args = (TwoArg*) vArgs;
  int fd = *(int *)Args->arg1;
  unsigned position = *(unsigned *)Args->arg2;
  //implement
}

void tell (void* vArgs){
  OneArg* Args = (OneArg*) vArgs;
  int fd = *(int*)Args->arg1;
  //implement
}

void close (void* vArgs){
  OneArg* Args = (OneArg*) vArgs;
  int fd = *(int *)Args->arg1;
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

void * load_args(void *esp){
  ThreeArg *Args = malloc(sizeof(ThreeArg));
  Args->arg1 = convert(validate(esp));
  Args->arg2 = convert(validate((void *)((int *)esp + 1)));
  Args->arg3 = convert(validate((void *)((int *)esp + 2)));
  return (void *) Args;
  // switch (call_code) {
  //   case SYS_HALT:
  //   case SYS_EXIT:
  //   case SYS_EXEC:
  //   case SYS_WAIT:
  //   case SYS_REMOVE:
  //   case SYS_OPEN:
  //   case SYS_FILESIZE:
  //   case SYS_TELL:
  //   case SYS_CLOSE:
  //     OneArg *oneArg = malloc(sizeof(OneArg));
  //     oneArg->arg1 = convert(validate(esp));
  //     return (void *) oneArg;
  //   case SYS_CREATE:
  //   case SYS_SEEK:
  //     TwoArg *twoArg = malloc(sizeof(TwoArg));
  //     twoArg->arg1 = convert(validate(esp));
  //     twoArg->arg2 = convert(validate((void *)((int *)esp + 1)));
  //     return (void *) twoArg;
  //   case SYS_READ:
  //   case SYS_WRITE:
  //     ThreeArg *threeArg = malloc(sizeof(ThreeArg));
  //     threeArg->arg1 = convert(validate(esp));
  //     threeArg->arg2 = convert(validate((void *)((int *)esp + 1)));
  //     threeArg->arg3 = convert(validate((void *)((int *)esp + 2)));
  //     return (void *) threeArg;
  // }
  // return NULL;
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  int call_code = *(int *)f->esp;
  // get args
  void* args = load_args(f->esp + 1);
  eax = &f->eax;

  systemCalls[call_code](args);
}
