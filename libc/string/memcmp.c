#include <string.h>
 
int memcmp(const void* aptr, const void* bptr, size_t size) {
    const unsigned char* a = (unsigned char*) aptr;
	const unsigned char* b = (const unsigned char*) bptr;
    int diff = 0;
    for(size_t i = 0; i < size; i++){
        diff = a[i] - b[i];
        if(diff > 0){
            return 1;
        }else if(diff < 0){
            return -1;
        }
    }
    return 0;
}