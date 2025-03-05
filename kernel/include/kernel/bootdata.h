#ifndef _KERNEL_BOOTDATA_H
#define _KERNEL_BOOTDATA_H
#include <stdbool.h>

#define FB_ADDR 0xFF800000

extern bool BD_CPU_SSE;
typedef  struct
{
    unsigned int mem_lower;
    unsigned int mem_upper;
    unsigned int fb_addr;
    unsigned int fb_pitch;
    unsigned int fb_width;
    unsigned int fb_height;
    unsigned char fb_bpp;
    unsigned char fb_type;
    unsigned int fb_info;
    unsigned int sse_en;
}   __attribute__((__packed__)) boot_data_t;

void cpu_data_init(boot_data_t* data);
#endif