#include <string.h>

int strncmp(const char* a, const char* b, size_t n) {
    int t = n;
    while (n > 0 && *a && *b)
    {

        if(*a != *b){
            return *a - *b;
        }
        a++;
        b++;
        n--;
    }
    if(*(a - 1) == *(b-1)){
        return 0;
    }else{
        return *a - *b;
    }
}