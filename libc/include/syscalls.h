#ifndef _SYSCALLS_H
#define _SYSCALLS_H 1
 
#include <sys/cdefs.h>
 
#ifdef __cplusplus
extern "C" {
#endif
 
__attribute__((__noreturn__))


extern int get_eax();

int write(int f_num, char *ptr, int len);
int lseek(int fd, int ptr, int dir);
int read(int f_num, char *ptr, int len);
int open(const char* name, int flags, int mode);
int close(int fd);

extern void syscall_4(int, int, int, int);

extern void syscall_3(int, int, int);
extern void syscall_2(int, int);
extern void syscall_1(int);

#ifdef __cplusplus
}
#endif
 
#endif