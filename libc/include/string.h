#ifndef _STRING_H
#define _STRING_H 1
 
#include <sys/cdefs.h>
 
#include <stddef.h>

 
#ifdef __cplusplus
extern "C" {
#endif
 
int memcmp(const void*, const void*, size_t);
void* memcpy(void* __restrict, const void* __restrict, size_t);
void* memmove(void*, const void*, size_t);
void* memset(void*, int, size_t);
size_t strlen(const char*);
void *malloc(size_t);
void free(void* location);
int strcmp(const char* a, const char* b);
int strncmp(const char* a, const char* b, size_t n);
char* strrchr(const char* s, int c);
char* strchr(const char* s, int c);
char* strncpy( char* dest, const char* src, size_t count );
char * strstr ( const char * str, const char * sub);
#ifdef __cplusplus
}
#endif
 
#endif