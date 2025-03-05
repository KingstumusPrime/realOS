#ifndef _CTYPE_H
#define _CTYPE_H 1
 
#include <sys/cdefs.h>
 
#include <stddef.h>
#include <string.h>

 
#ifdef __cplusplus
extern "C" {
#endif

// converts a charecter to lower case
int tolower ( int c );
int toupper ( int c );
int isspace ( int c );
#ifdef __cplusplus
}
#endif
 
#endif