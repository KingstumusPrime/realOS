#include <stdint.h>
#include <stddef.h>
#include <kernel/inline.h>


void io_wait(void)
{
    outb(0x80, 0);
}
