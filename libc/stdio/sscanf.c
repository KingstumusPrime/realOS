// NOT REAL STREAM STANDARDS

#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#if defined(__is_libk)
#include <kernel/tty.h>
#endif


int scanf_int(const char* str, int* v){
    int ch = 0;
    while(isspace(*str)){str++;ch++;}
    
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
        ch++;
    }
    *v = res * neg;
	return ch;
}


int scanf_hex(const char* str, int* v){
    int ch = 0;

    while(isspace(*str)){str++;ch++;}
    int x = 0; // has an X been seen
    while(*str == '0' || (x == 0 && (*str == 'x' || *str == 'X'))){

        if(*str == 'x' || *str == 'X'){
            x = 1;  
        }
        str++;
        ch++;
    }
    
    int res = 0;
    while(
    (*str >= '0' && *str <= '9')
    || (*str >= 'a' && *str <= 'f')
    || (*str >= 'A' && *str <= 'F')){
        if((*str >= '0' && *str <= '9')){
            res = (res * 16) + *str - '0'; 
        }else if((*str >= 'a' && *str <= 'f')){
            res = (res * 16) + (*str - 'a') + 10;    
        }else{
            res = (res * 16) + (*str - 'A') + 10; 
        }
        str++;
        ch++;
    }
    
    
    *v = res;
	return ch;
}


int sscanf(const char* restrict s, const char* restrict format, ...){
    va_list parameters;
	va_start(parameters, format);

	int assigned = 0;
    while (*format != '\0')
    {
        if(*format == '%'){
            format++;
            switch (*format){
                case 'X':
                case 'x':
                    s += scanf_hex(s, va_arg(parameters, int*));
				    assigned++;
                    break;
                case 'i':
                case 'd':
                case 'u':
                    s += scanf_int(s, va_arg(parameters, int*));
                    assigned++;
                    break;
                    
            }
		    format++;
        }else{
			format++;
		}
    }
    va_end(parameters);
	if(assigned == 0){
		return EOF;
	}else{
	    return assigned;
	}

}