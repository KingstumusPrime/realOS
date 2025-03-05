#ifndef _STRINGS_H
#define _STRINGS_H 1
 
#include <sys/cdefs.h>
 
#include <stddef.h>
#include <string.h>

 
#ifdef __cplusplus
extern "C" {
#endif

// mallocs the exact string
char* strdup(const char* s);

int strcasecmp(const char* a, const char* b);
int strncasecmp(const char* a, const char* b, size_t n);

#ifdef __cplusplus
}
#endif
 
#endif