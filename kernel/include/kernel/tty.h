#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H
 
#include <stddef.h>

void tty_terminal_initialize(void);
void tty_terminal_putchar(char c);
void tty_terminal_write(const char* data, size_t size);
void tty_terminal_writestring(const char* data);
void tty_terminal_scroll_down();
void tty_terminal_scroll_up();

extern size_t terminal_row;
extern size_t terminal_column;

#endif