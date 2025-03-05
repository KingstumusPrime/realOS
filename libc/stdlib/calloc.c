#include <stdlib.h>
#include <string.h>


void *calloc(size_t num, size_t size){
    printf("CALLOC OF: %i %i %i\n", num, size, (num * size));
    char* p = malloc(num * size);
    memset(p, 0, num * size);
    printf("returning %X", p);
    return p;
}