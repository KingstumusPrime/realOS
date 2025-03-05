#include <stdlib.h>

// replace with scanf_int later
int atoi(const char* str){
    while(isspace(*str)){str++;}

    int res = 0;
    int digitV = 1; // digit value
    int neg = 1;  
    if(*str == '-'){
        neg = -1;
        str++;
    }else if(*str == '+'){
        str++;
    }

    while (*str >= '0' && *str <= '9')
    {
        res = res * 10 + (*str - '0');
        str++;
    }
    return res * neg;
}