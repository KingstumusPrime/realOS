#include <stdio.h>
#include <kernel/syscalls/sysfile.h>
#include <kernel/syscalls/sysui.h>
#include <kernel/syscalls/sysmem.h>
#include <kernel/pit.h>
typedef struct {

    unsigned int ebx;
    unsigned int ecx;
    unsigned int edx;
    unsigned int esi;
    unsigned int edi;
    unsigned int ebp;
    unsigned int validate;
    unsigned int ds;
    unsigned int es;
    unsigned int fs;
    unsigned int gs;
    unsigned int eax;
} __attribute__((__packed__))  syscall_args;

void syscall_c(syscall_args* arg){
    // printf("eax %X\ngs %X\nfs %X\nes %X\nds %X\nvalidate %X\nebp %X\nedi %X\nesi %X\nedx %X\necx %X\nebx %X\nesp %X\n",
    // arg->eax, arg->gs, arg->fs, arg->es, arg->ds, arg->validate,
    // arg->ebp, arg->edi, arg->esi, arg->edx, arg->ecx, arg->ebx,
    // arg->eax);
    switch (arg->eax)
    {
    // put char
    case 1:
        putchar(arg->ebx);
        break;
    case 2:
        mov_eax(kwrite(arg->ebx, arg->ecx, arg->edx));
        break;
    case 3:
        mov_eax(klseek(arg->ebx, arg->ecx, arg->edx));
        break;
    case 4:
        int x = kread(arg->ebx, arg->ecx, arg->edx);

        break;
    case 5:
        mov_eax(kopen(arg->ebx, arg->ecx, arg->edx));
        break;
    case 6:
        mov_eax(kclose(arg->ebx));
        break;
    case 16:
        mov_eax(ticks);
        break;
    case 17:
        mov_eax(kioctl(arg->ebx, arg->ecx, arg->edx));
        break;
    case 19:  
        mov_eax(syscpy(arg->ebx, arg->ecx, arg->edx));
        break;
    case 20:
        mov_eax(kfb_set(arg->ebx, arg->ecx, arg->edx));
        break;
    case 21:
        mov_eax(kfb_cpy(arg->ebx, arg->ecx, arg->edx));
        break;
    default:
        printf("UNKOWN SYSCALL %X\n", arg->eax);
        abort();
        break;
    }

}