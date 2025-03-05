#ifndef _KERNEL_KEYBOARD_H
#define _KERNEL_KEYBOARD_H
#include <stddef.h>
#include <kernel/inline.h>
 

extern unsigned char keyboard_buffer[256];
extern uint8_t keyboard_buffer_index;

void keyboard_handler(struct regs *r);


#endif