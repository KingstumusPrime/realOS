#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#if defined(__is_libk)
    #include <kernel/syscalls/sysmem.h>
#else
#include <syscalls.h>
extern void syscall_4(int, int, int, int);
#endif


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



void* memcpy(void* __restrict dstptr, const void* __restrict srcptr, size_t size) {
    #if defined(__is_libk)
        syscpy(dstptr, srcptr, size);
    
        return dstptr;
    #else    

        syscall_4(19, dstptr, srcptr, size);

        unsigned int value; 
        __asm__ volatile("mov %%eax, %0" : "=r"(value)); 


        return value;

        
    #endif

}