#ifndef _RENDERER_H
#define _RENDERER_H 1

 
#ifdef __cplusplus
extern "C" {
#endif


#define WHITE 0xFFFFFFFF

extern unsigned int resX;
extern unsigned int resY;
extern unsigned int bpp;
extern unsigned char* buffer;

typedef struct
{
    short x; // x pos of the window
    short y; // y pos of the window
    short w; // width
    short h; // height
    unsigned char* buffer; // the framebuffer for the window
} window;


#ifdef __cplusplus
}
#endif
 
#endif