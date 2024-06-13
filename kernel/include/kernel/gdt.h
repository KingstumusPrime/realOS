#ifndef _KERNEL_GDT_H
#define _KERNEL_GDT_H
 
#include <stddef.h>
#include <stdint.h>

void gdt_install();

// reload segment regs
extern void gdt_flush();

void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran);
#endif