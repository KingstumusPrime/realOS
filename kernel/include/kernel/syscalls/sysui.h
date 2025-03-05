#ifndef _KERNEL_SYSUI_H
#define _KERNEL_SYSUI_H
 
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <kernel/drivers/fb_terminal.h>

int kfb_set(int off, int v, int sz);
int kfb_cpy(int off, char* buf, int sz);
#endif