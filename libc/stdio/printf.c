#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
static bool print(const char* data, size_t length) {
    // returns false on eof
	const unsigned char* bytes = (const unsigned char*) data;
	for(size_t i = 0; i < length; i++){
	    if(putchar(bytes[i]) == EOF){
	        return false;
	    };
	}
	return true;
}

int printf(const char* restrict format, ...) {
    // just printf
	va_list parameters;
	va_start(parameters, format);
	int chCount = 1;
    while (*format != '\0')
    {
        if(*format == '%'){
            format++;
            switch (*format){
                case'c':
                    putchar(va_arg(parameters, int));
                    format++;
                    break;
                case 's':
                    const char* str = va_arg(parameters, const char*);
                    int l = strlen(str);
                    if(!print(str, l)){
                        // overflow error
                        print("overflow error!", 16);
                        return -1;
                    }else{
                    format++;
                    chCount += l;
                    }

                    break;
            }
        }else{
            putchar(*format);
            format++;
            chCount++;

        }

    }
    
    return chCount;
}
