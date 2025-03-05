#ifndef _KERNEL_SYSMEM_H
#define _KERNEL_SYSMEM_H
 
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <kernel/bootdata.h>

void* syscpy(void* dstptr, const void* srcptr, size_t size);
#endif