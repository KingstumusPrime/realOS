#include <string.h>

int strcmp(const char* a, const char* b) {
    while (*a != '\0')
    {
        if(*a != *b){
            return *a - *b;
        }
        a++;
        b++;
    }
    if(*a == *b){
        return 0;
    }else{
        return *a - *b;
    }
    
}