#include <ui.h>
#include <syscalls.h>
#include <string.h>



extern void syscall_3(int, int, int);


// memset
int fb_set(int off, int v, int sz){

    syscall_4(20, off, v, sz);

    
    unsigned int value; 
    __asm__ volatile("mov %%eax, %0" : "=r"(value)); 

    return value;
}

// memcpy
int fb_cpy(int off, char* buf, int sz){
    syscall_4(21, off, buf, sz);

    unsigned int value; 
    __asm__ volatile("mov %%eax, %0" : "=r"(value)); 

    return value;
}