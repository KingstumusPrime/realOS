#ifndef _STDLIB_H
#define _STDLIB_H 1
 
#include <sys/cdefs.h>
#include <math.h>
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
 
__attribute__((__noreturn__))
void abort(void);
void exit(int status);
int atoi(const char* str);
// stub
double atof(const char* str);
void *calloc(size_t num, size_t size);
void* realloc(void* addr, size_t size);
#ifdef __cplusplus
}
#endif
 
#endif