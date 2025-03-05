#ifndef _KERNEL_FB_TERM_H
#define _KERNEL_FB_TERM_H
#include <stddef.h>
#include <stdint.h>
#include <kernel/bootdata.h>

extern int FB_TERM_COLOR;
void fb_term_init(boot_data_t* data);
void buffer_putchar(char c);
void buffer_write(const char* data, size_t size);
void buffer_write_string(const char* data);
void buffer_scroll_down();
void buffer_scroll_up();

extern int fb_term_width;
extern int fb_term_height;
extern int fb_term_bpp;

#endif