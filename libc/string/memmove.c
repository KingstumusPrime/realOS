#include <string.h>
 
void* memmove(void* dstptr, const void* srcptr, size_t size) {
    unsigned char* dst = (unsigned char*) dstptr;
	const unsigned char* src = (const unsigned char*) srcptr;
    for(size_t i = 0; i < size; i++){
        if(dst < src){
            dst[i] = src[i];
        }else{
            dst[size - i - 1] = src[size - i - 1];
        }
    }
    return dstptr;
}