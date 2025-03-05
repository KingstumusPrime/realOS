#include <stdio.h>
#include <renderer.h>
#include <bmp.h>
#include <string.h>
#include <stdlib.h>
#include <ui.h>


// make this dynamic
unsigned int resX = 1024;
unsigned int resY = 768;
unsigned int bpp = 4;

unsigned char* buffer;

void plotWindow(window* w){
    for(int h = 0; h < w->h; h++){
        if(h < 5 || h > w->h - 5){
            memset(buffer + ((w->y + h) * resX * bpp) + (w->x * bpp), 0xFF, (w->w * bpp));
        }else{
            memcpy(buffer + ((w->y + h) * resX * bpp) + (w->x * bpp), w->buffer + (h * w->w * bpp), (w->w * bpp));
            // side borders
            ((int*)(buffer + ((w->y + h) * resX * bpp) + (w->x * bpp)))[0] = WHITE;
            ((int*)(buffer + ((w->y + h) * resX * bpp) + ((w->w + w->x - 1) * bpp)))[0] = WHITE;
        }
    }

}

void moveWindowBy(window* w, int x, int y){
    if(y > 0){
        for(int h = 0; h < y; h++){
            memset(buffer + ((w->y + h) * resX * bpp) + (w->x * bpp), 0, (w->w * bpp));
        }
    }else{
        for(int h = w->h; h > w->h + y; h--){

            
            memset(buffer + ((w->y + h) * resX * bpp) + (w->x * bpp), 0, (w->w * bpp));
        }
    }

    w->x += x;
    w->y += y;
    plotWindow(w);
}

int main(){
    stdout = fopen("/dev/tty", "w");
    // printf("HELLO WORLD!\n");
    buffer = malloc(resX * resY * bpp);
    for(int i = 0; i < resX * resY; i++){
        ((int*)buffer)[i] = 0x0;
    }


    // for(int i =0; i < 80; i++){

    //     moveWindowBy(&test, 0, 1);
    //     fb_cpy(0, buffer, resX * resY * bpp);
    // }

    // printf("HELLO WORLD!\n");

    FILE* bmp = fopen("/RED.BMP", "w");
    if(!bmp){
        printf("FILE NOT FOUND!\n");
        abort();
    }

    bmp_header_t* bmp_head = malloc(sizeof(bmp_header_t));
    bmp_dib_t* bmp_dib = malloc(sizeof(bmp_dib_t));

    fread(bmp_head, 1, sizeof(bmp_header_t), bmp);
    fread(bmp_dib, 1, sizeof(bmp_dib_t), bmp);


    printf("=== BMP DATA ===\n");
    printf("ID %c%c\n", (unsigned char)bmp_head->id, bmp_head->id >> 8);
    printf("===   DIB    ===\n");
    printf("WIDTH %X\nHEIGHT %X\n", bmp_dib->width, bmp_dib->height);
    printf("NUM   %X\n", bmp_dib->bitmap_size);
    printf("BPP   %X\n", bmp_dib->bpp);
    printf("SIZE  %X\n", bmp_dib->bitmap_size);

    unsigned char* mbuf = malloc(bmp_dib->height * bmp_dib->width * 3);
    //fseek(bmp, bmp_head->data_offset, SEEK_SET);
    fread(mbuf, 1, bmp_dib->height * bmp_dib->width * 3, bmp);


    window test;
    test.x = 200;
    test.y = 0;
    test.w =  bmp_dib->width;
    test.h =  bmp_dib->height;
    test.buffer = malloc(test.w * test.h * bpp);
    memset(test.buffer, 0x10, test.w * test.h * bpp);

    int i = 0;
    for(int j = bmp_dib->height; j > 0;){
        for(int w = 0; w < bmp_dib->width * 3;){
            test.buffer[i + 0] = mbuf[(j * 3 * bmp_dib->width) + w++];
            test.buffer[i + 1] = mbuf[(j * 3 * bmp_dib->width) + w++];
            test.buffer[i + 2] = mbuf[(j * 3 * bmp_dib->width) + w++];
            test.buffer[i + 3] = 0;
            i += 4;

            
        }
        j -= 1;
    }
    free(mbuf);
    fclose(bmp);

    bmp = fopen("/BG.BMP", "w");
    if(!bmp){
        printf("FILE NOT FOUND!\n");
        abort();
    }
    
    fread(bmp_head, 1, sizeof(bmp_header_t), bmp);
    fread(bmp_dib, 1, sizeof(bmp_dib_t), bmp);


    printf("=== BMP DATA ===\n");
    printf("ID %c%c\n", (unsigned char)bmp_head->id, bmp_head->id >> 8);
    printf("===   DIB    ===\n");
    printf("WIDTH %X\nHEIGHT %X\n", bmp_dib->width, bmp_dib->height);
    printf("NUM   %X\n", bmp_dib->bitmap_size);
    printf("BPP   %X\n", bmp_dib->bpp);
    printf("SIZE  %X\n", bmp_dib->bitmap_size);

    mbuf = malloc(bmp_dib->height * bmp_dib->width * 4);
    fseek(bmp, bmp_head->data_offset, SEEK_SET);
    fread(mbuf, 1, bmp_dib->height * bmp_dib->width * 4, bmp);


    window test1;
    test.x = 0;
    test1.y = 0;
    test1.w =  bmp_dib->width;
    test1.h =  bmp_dib->height;
    test1.buffer = malloc(test1.w * test1.h * bpp);
    memset(test1.buffer, 0x10, test1.w * test.h * bpp);

    i = 0;
    for(int j = bmp_dib->height; j > 0;){
        for(int w = 0; w < bmp_dib->width * 4;){
            test1.buffer[i + 0] = mbuf[(j * 4 * bmp_dib->width) + w++];
            test1.buffer[i + 1] = mbuf[(j * 4 * bmp_dib->width) + w++];
            test1.buffer[i + 2] = mbuf[(j * 4 * bmp_dib->width) + w++];
            test1.buffer[i + 3] = mbuf[(j * 4 * bmp_dib->width) + w++];
            i += 4;

            
        }
        j -= 1;
    }
    free(mbuf);
    unsigned char count;
    while (1)
    {  
        count++;
        test.x = count;
        plotWindow(&test1);
        plotWindow(&test);
        fb_cpy(0, buffer, resX * resY * bpp);
    }
    
    return 0;
}