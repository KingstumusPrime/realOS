#include <system.h>
#include <syscalls.h>

unsigned long long time(){
    syscall_1(16);
    unsigned int value; 
    __asm__ volatile("mov %%eax, %0" : "=r"(value)); 
    return value;
}