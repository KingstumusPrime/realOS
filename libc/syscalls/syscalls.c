#include <syscalls.h>


extern void mov_eax(int);
extern void mov_ebx(int);
extern void mov_ecx(int);
extern void mov_edx(int);
extern void call_int(int);
extern void asm_open();



int write(int f_num, char *ptr, int len){
    syscall_4(2,f_num, ptr, len);
    
    unsigned int value; 
    __asm__ volatile("mov %%eax, %0" : "=r"(value)); 
    return value;
}


int lseek(int fd, int ptr, int dir){
    syscall_4(3, fd, ptr, dir);

    unsigned int value; 
    __asm__ volatile("mov %%eax, %0" : "=r"(value)); 

    return value;
}

int read(int f_num, char *ptr, int len){
    syscall_4(4, f_num, ptr, len);
    unsigned int value;
    __asm__ volatile("mov %%eax, %0" : "=r"(value)); 

    
    return value;
}


int open(const char* name, int flags, int mode){
    syscall_4(5, name, flags, mode);
    unsigned int value; 
    __asm__ volatile("mov %%eax, %0" : "=r"(value)); 

    return value;
}

int close(int fd){
    syscall_2(6, fd);
    unsigned int value; 
    __asm__ volatile("mov %%eax, %0" : "=r"(value)); 

    return value;
}

