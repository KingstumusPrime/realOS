#include <string.h>

char* strncpy( char* dest, const char* src, size_t count ){
    int i = 0;
    while (count > 0)
    {
        dest[i] = src[i];
        if(src[i] == '\0'){

            break;
        }
        count--;
        i++;
    }
    while (count > 0)
    {
        dest[i] = '\0';
        count--;
        i++;
    }
    return dest;
}