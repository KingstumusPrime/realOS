#ifndef _UCHAR_H
#define _UCHAR_H 1
 
#include <sys/cdefs.h>
 
#include <stddef.h>
#include <uchar.h>
 
#ifdef __cplusplus
extern "C" {
#endif
 
typedef struct{
    unsigned int byte_pos;
    unsigned int state_flags;
} shift_state_t;

typedef struct
{
    unsigned processed_bytes;
    shift_state_t shift_state;
    unsigned incomplete[];

} mbstate_t;

 
#ifdef __cplusplus
}
#endif
 
#endif