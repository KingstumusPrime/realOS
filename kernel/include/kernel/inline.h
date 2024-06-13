#ifndef _KERNEL_INLINE_H
#define _KERNEL_INLINE_H
 
#include <stddef.h>
#include <stdint.h>

extern void clearInterrupts();
extern void setInterrupts();

void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);

void io_wait(void);
#endif