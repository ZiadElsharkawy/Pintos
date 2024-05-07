#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

void syscall_exit(int status);
void syscall_halt(void);
static void syscall_handler(struct intr_frame *);
static pid_t exec (const char *);

void syscall_init(void)
{
  intr_register_int(0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler(struct intr_frame *f ) // 1.halt 2.exit 3.exec 4.wait 5.create 6.remove 7.open 8.filesize 9.read 10.write 11.seek 12.tell 13.close
{
  int syscall = (int)f->esp;
  //printf ("system call!\n");
  if (syscall == SYS_HALT){
     halt();
     break;
   } else if (syscall == SYS_EXIT){
            exit(*(int *)(f->esp + 3));
            break;
   }else if (syscall == SYS_EXEC) {
    pid_t child_pid = exec((char *)(f->esp + 4));
    f->eax = child_pid;
    break;
}
  else if (syscall == SYS_WAIT){

  }else if (syscall == SYS_CREATE){

  }else if (syscall == SYS_REMOVE){

  }else if (syscall == SY_OPEN){
  }else if (syscall == SYS_FILESIZE){
   }else if (syscall == SYS_READ){

  }else if (syscall == SYS_WRITE){
    ()sp;
    
      file_write()
   }else if (syscall == SYS_SEEK){

  }else if (syscall == SYS_TELL){

   }else if (syscall == SYS_CLOSE){

   }
    handle_exit();

}
void
halt (void)
{
  shutdown_power_off ();
}
// Terminates the current user program, returning status to the kernel. If the process's
//  parent waits for it (see below), 
// this is the status that will be returned. Conventionally,
//  a status of 0 indicates success and nonzero values indicate errors.
void exit (int status)
{
     struct thread *cur = thread_current ();
       printf ("%s: exit(%d)\n", cur->name, status);
       process_exit(status);
   
thread_exit();
}
pid_t
exec(const char *cmd_line)
{
    // Check if cmd_line is a valid  pointer
    if (!is_user_vaddr(cmd_line) || cmd_line == NULL || !is_user_vaddr(cmd_line + strlen(cmd_line)))
        return -1;

    // Initialize a semaphore to synchronize between parent and child
    struct semaphore exec_sema;
    sema_init(&exec_sema, 0);

    // Execute the process and pass the semaphore to process_execute
    pid_t child_pid = process_execute(cmd_line, &exec_sema);

    // Wait for the child to load the executable
    sema_down(&exec_sema);

    // If child_pid is still less than 0, process_execute failed
    if (child_pid < 0)
        return -1;

    // Return the child process's pid
    return child_pid;
}
