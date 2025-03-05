#include <stdlib.h>

// no supprot for scientific notation
double atof(const char* str){
    while(isspace(*str)){str++;}


    double res = 0;
    int digitV = 1; // digit value
    int neg = 1;  
    if(*str == '-'){
        neg = -1;
        str++;
    }else if(*str == '+'){
        str++;
    }

    // number section
    while (*str >= '0' && *str <= '9')
    {
        res = res * 10 + (*str - '0');
        str++;
    }
    double dec = 0;
    
    if(*str == '.'){
        str++;
        int d = 10;
        while (*str >= '0' && *str <= '9')
        {
            dec += ((double)(*str - '0')/d);
            str++;
            d *= 10;
        }
    }
    
    return (res + dec) * neg;
}