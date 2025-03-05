#ifndef _TTY_H
#define _TTY_H

#include <kernel/syscalls/sysfile.h>
#include <termios.h>

size_t tty_read(file* f, char* buffer, size_t size,  size_t* off);
size_t tty_read(file* f, char* buffer, size_t size,  size_t* off);
size_t tty_write(file* f, char* buffer, size_t size,  size_t* off);
size_t tty_ioctl(file* file, int func, void* d);
int tty_open(inode* node, file* target);

typedef struct
{
    struct termios* term;
    char* buf;
    char* bufsz;
    unsigned short bpos;
} tty_device;


extern file_operations tty_ops;
#endif