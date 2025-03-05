#include <kernel/syscalls/sysmem.h>

static inline void* memcpy_basic(void* dstptr, const void* srcptr, size_t size) {

    unsigned int* dst = (unsigned int*) dstptr;
	const unsigned int* src = (const unsigned int*) srcptr;


    unsigned int num32 = size/4;
    unsigned int num8 = size%4;
    for(size_t i = 0; i < num32; i++){
        *dst = *src;
        dst++;
        src++;
    }
    
    unsigned char* dst8 = (unsigned char*) dst;
	const unsigned char* src8 = (const unsigned char*) src;
 
    for(int i = 0; i < num8; i++){
        *dst8 = *src8;
        dst8++;
        src8++;
    }
    return dstptr;
    // unsigned char* dest8 = (unsigned char*) dstptr;
	// const unsigned char* src8 = (const unsigned char*) srcptr;
    // for(int i = 0 ; i < size; i++){
    //     dest8[i] = src8[i];
    // }
}



void* syscpy(void* dstptr, const void* srcptr, size_t size) {
    if(BD_CPU_SSE){
        if(size%128 == 0){
            void* src = srcptr;
            for(int i = 0; i < size/16; i++){
                __asm__ __volatile__ ("movups (%0), %%xmm0\n" "movntdq %%xmm0, (%1)\n"::"r"(src), "r"(dstptr) : "memory");
                dstptr += 16;
                src += 16;
            }

        }else{
            memcpy_basic(dstptr, srcptr, size);
        }

    }else{

        memcpy_basic(dstptr, srcptr, size);
    }

    return dstptr;
}