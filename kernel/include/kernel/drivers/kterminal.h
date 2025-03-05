#ifndef _KERNEL_KTERM_H
#define _KERNEL_KTERM_H
#include <stddef.h>
#include <stdint.h>
#include <kernel/bootdata.h>

#define FB_ADDR 0xFF800000

extern int fb_height;
extern int fb_width;
extern int fb_bpp;
// set by terminal_init
extern void (*terminal_putchar)(char);
extern void (*terminal_write)(const char*, size_t);
extern void (*terminal_write_string)(const char*);
extern void (*terminal_scroll_down)();
extern void (*terminal_scroll_up)();
void terminal_init(boot_data_t* data);


#endif