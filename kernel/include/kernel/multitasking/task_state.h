#ifndef _KERNEL_PROCESS_H
#define _KERNEL_PROCESS_H

#include <kernel/syscalls/sysfile.h>




typedef struct 
{
    unsigned int* esp;
    unsigned int* cr3;
    unsigned int* esp0;
    volatile long state;
    unsigned long flags; 
    unsigned int addr_limit; // 0-0xBFFFFFFF for user-thead full range for kernel
    unsigned int pid;
    unsigned int tid;
    unsigned int exit_code;

    unsigned int tty; // the tty descriptor

    // file system
    int max_fds;
    file** fdt;
    void* pc;    
} __attribute__((__packed__)) task_state ;



extern task_state* current_task;
extern task_state** pqueue;
extern unsigned int pqueue_pos;
extern unsigned int pqueue_size;

#define current (current_task)


#endif