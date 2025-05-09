#include "userprog/syscall.h"
#include <stdio.h>
#include <stdlib.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/synch.h"
#include "filesys/file.h"
#include "filesys/filesys.h"
#include "threads/vaddr.h"

#define ERROR -1
#define STDIN_FILENO 0
#define STDOUT_FILENO 1

struct lock filesys_lock;

static void syscall_handler (struct intr_frame *);

uint32_t *eax;

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
  lock_init(&filesys_lock);
}

typedef void (*SystemCall)(Arguments*);

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

void halt (Arguments *args){
  // printf("(halt) begin");
  shutdown_power_off();
}

void exit (Arguments *args){
  int status = *(int*) args->arg1;
  //implement
}

void exec (Arguments *args){
  const char *cmd_line = (char*) args->arg1;
  *eax = process_execute(cmd_line);
}

void wait (Arguments *args){
  tid_t pid = *(tid_t*) args->arg1;
  *eax = process_wait(pid);
}

void create (Arguments *args){
  const char *file = (char*) args->arg1;
  unsigned initial_size = *(unsigned*) args->arg2;
  
  lock_acquire(&filesys_lock);
  *args->eax = filesys_create(file, initial_size);
  lock_release(&filesys_lock);
}

void remove (Arguments *args){
  const char *file = (char*) args->arg1;

  lock_acquire(&filesys_lock);
  *args->eax = filesys_remove(file);
  lock_release(&filesys_lock);
}

void open (Arguments *args){
  const char *filename = (char*) args->arg1;

  lock_acquire(&filesys_lock);
  struct file * file = filesys_open(filename);
  lock_release(&filesys_lock);

  if(file == NULL){
    *args->eax = ERROR;
    return;
  }

  struct opened_file * opened_file = (struct opened_file *) malloc(sizeof(struct opened_file));
  struct thread * current = thread_current();
  opened_file->file = file;
  opened_file->fd = current->next_fd++;
  list_push_front(&current->opened_files, &opened_file->elem);

  *args->eax = opened_file->fd;      /* return fd by assignning it to the eax */
}

struct file* get_file(int file_descriptor){
	struct list_elem *e;

	for (e = list_begin(&thread_current()->opened_files); e != list_end(&thread_current()->opened_files);e = list_next (e))
	{
    struct opened_file *curr = list_entry(e,struct opened_file,elem);
    if(curr->fd == file_descriptor){
      return curr->file;
    }
  }
  return NULL;
}

void filesize (Arguments *args){
  int fd = *(int*) args->arg1;
  struct file *file = get_file(fd);
  if(file == NULL){
    *args->eax = ERROR;
    return;
  }
  lock_acquire(&filesys_lock);
  *args->eax = file_length(file);
  lock_release(&filesys_lock);
}

void read (Arguments *args){
  int fd = *(int*) args->arg1;
  void *buffer = args->arg2;
  unsigned size = *(unsigned*) args->arg3;

  if (fd == STDIN_FILENO) {
    for (unsigned i = 0; i < size; i++) {
      lock_acquire(&filesys_lock);
      ((char *)buffer)[i] = input_getc();
      lock_release(&filesys_lock);
    }
    *args->eax = size;
    return;
  }
  else{
    struct file *file = get_file(fd);
    if(file == NULL){
      *args->eax = ERROR;
    }
    
    lock_acquire(&filesys_lock);
    *args->eax = file_read(file, buffer, size);
    lock_release(&filesys_lock);
  }
}

void write (Arguments *args){
  int fd = *(int*) args->arg1;
  const void *buffer = args->arg2;
  unsigned size = *(unsigned*) args->arg3;

  if (fd == STDOUT_FILENO) {
    lock_acquire(&filesys_lock);
    putbuf(buffer, size);
    lock_release(&filesys_lock);
    *args->eax = size;
    return;
  } 
  else {
    struct file *file = get_file(fd);
    if(file == NULL){
      *args->eax = ERROR;
      return;
    }
    lock_acquire(&filesys_lock);
    *args->eax = file_write(file, buffer, size);
    lock_release(&filesys_lock);
  }
}

void seek (Arguments *args){
  int fd = *(int *) args->arg1;
  unsigned position = *(unsigned *) args->arg2;
  struct file *file = get_file(fd);
  
  lock_acquire(&filesys_lock);
  file_seek(file, position);
  lock_release(&filesys_lock);
}

void tell (Arguments *args){
  int fd = *(int *) args->arg1;
  struct file *file = get_file(fd);
  
  lock_acquire(&filesys_lock);
  *args->eax = file_tell(file);
  lock_release(&filesys_lock);
}

void close (Arguments *args){
  int fd = *(int *) args->arg1;
  struct file *file = get_file(fd);

  lock_acquire(&filesys_lock);
  file_close(file);
  lock_release(&filesys_lock);
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

Arguments * load_args(void *esp, uint32_t *eax){
  Arguments *args = malloc(sizeof(Arguments));
  args->arg1 = convert(validate(esp));
  args->arg2 = convert(validate((void *)((int *)esp + 1)));
  args->arg3 = convert(validate((void *)((int *)esp + 2)));
  args->eax = eax;
  return args;
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  int call_code = *(int *)f->esp;
  Arguments *args = load_args(f->esp + 1, &f->eax);
  systemCalls[call_code](args);
}