#ifndef _UNISTD_H
#define _UNISTD_H 1
 
#include <sys/cdefs.h>
#include <sys/types.h>
 
#ifdef __cplusplus
extern "C" {
#endif
 
unsigned int sleep(unsigned int seconds);
int execv(const char*, char* const[]);
int execve(const char*, char* const[], char* const[]);
int execvp(const char*, char* const[]);
pid_t fork(void);

#ifdef __cplusplus
}
#endif
 
#endif