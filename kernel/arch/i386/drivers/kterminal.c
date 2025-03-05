#include <kernel/drivers/kterminal.h>
#include <kernel/bootdata.h>
#include <kernel/drivers/fb_terminal.h>
#include <kernel/tty.h>

void (*terminal_putchar)(char) = NULL;
void (*terminal_write)(const char*, size_t) = NULL;
void (*terminal_write_string)(const char*) = NULL;
void (*terminal_scroll_down)() = NULL;
void (*terminal_scroll_up)() = NULL;

int fb_height;
int fb_width;
int fb_bpp;

void terminal_init(boot_data_t* data){
    if(data->fb_addr == 0xB8000){
        tty_terminal_initialize();
        terminal_putchar = &tty_terminal_putchar;
        terminal_write = &tty_terminal_write;
        terminal_write_string = &tty_terminal_writestring;
        terminal_scroll_down = &tty_terminal_scroll_down;
        terminal_scroll_up = &tty_terminal_scroll_up;

        fb_width = terminal_row;
        fb_height = terminal_column;
        fb_bpp = 1;
    }else{
        fb_term_init(data);
        terminal_putchar = &buffer_putchar;
        terminal_write = &buffer_write;
        terminal_write_string = &buffer_write_string;
        terminal_scroll_down = &buffer_scroll_down;
        terminal_scroll_up = &buffer_scroll_up;

        fb_width = fb_term_width;
        fb_height = fb_term_height;
        fb_bpp = fb_term_bpp;
    }
}