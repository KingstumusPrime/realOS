#ifndef _KERNEL_IDT_H
#define _KERNEL_IDT_H
 
#include <stddef.h>
#include <stdint.h>

void idt_install();
void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags);
#endif