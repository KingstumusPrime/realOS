#include <kernel/syscalls/sysui.h>
#include <kernel/drivers/kterminal.h>

// memset
int kfb_set(int off, int v, int sz){

    if(sz + off <= fb_height *fb_width * fb_bpp){
        return memset(off + FB_ADDR, v, sz);
    }else{
        return 0;
    }

}

// memcpy
int kfb_cpy(int off, char* buf, int sz){


    if(sz + off <= fb_height * fb_width * fb_bpp){
       
        return memcpy(FB_ADDR + off, buf, sz);
    }else{
        printf("BAD BUFFER ACCESS\n");
        abort();
        return 0;
    }
}