#ifndef _KERNEL_PIT_H
#define _KERNEL_PIT_H
 
#include <stddef.h>
#include <kernel/inline.h>
extern unsigned long long ticks;
void PIT_handler(struct regs *r);
void PIT_config(int hz);
unsigned read_pit_count();
#endif