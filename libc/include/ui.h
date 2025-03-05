#ifndef _UI_H
#define _UI_H 1

 
#ifdef __cplusplus
extern "C" {
#endif

// memset
int fb_set(int off, int v, int sz);

// memcpy
int fb_cpy(int off, char* buf, int sz);

#ifdef __cplusplus
}
#endif
 
#endif