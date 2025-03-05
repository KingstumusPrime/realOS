#ifndef _BMP_H
#define _BMP_H 1

 
#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    unsigned short id;
    unsigned int file_size;
    unsigned int reserved;
    unsigned int data_offset;
} __attribute__((packed)) bmp_header_t;



typedef struct
{
    unsigned int num_bytes;
    unsigned int width;
    unsigned int height;
    unsigned short planes;
    unsigned short bpp;
    unsigned int compression;
    unsigned int bitmap_size;
    unsigned int dpi_x;
    unsigned int dpi_y;
    unsigned int colors;
    unsigned int important_colors;
} __attribute__((packed)) bmp_dib_t;

#ifdef __cplusplus
}
#endif
 
#endif