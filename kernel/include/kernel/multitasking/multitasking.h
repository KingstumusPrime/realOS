#ifndef _KERNEL_MULTITASKING_H
#define _KERNEL_MULTITASKING_H

#include <kernel/syscalls/sysfile.h>

#define MAX_PROCCESS_NUM 4

void multitasking_init();
void add_task(void* func);

#endif