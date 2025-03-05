#ifndef _KERNEL_FONTS_H
#define _KERNEL_FONTS_H

#define FONT_NAME_SIZE 8

typedef struct
{
    unsigned int size;
    unsigned char width; // width pixel
    unsigned char height; // height pixels
    unsigned char name[FONT_NAME_SIZE];
    unsigned char* data; // points to the actual font
} font_data_t;

extern font_data_t f_vga_basic;

#endif