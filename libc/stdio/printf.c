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


void print_hex(unsigned int num, bool upper){
    bool printing = false;
    for(int i = 0; i < 8; i++){
        if(!printing){
            if( (num & ((0xF0000000) >> (i * 4))) > 0){
                printing = true;
            }
        }
        if(printing){
            if(upper){
                printf("%c", hex_digits[ ((num & ((0xF0000000) >> (i * 4))) >> (28 - (i * 4)))]);
            }else{
                printf("%c", hex_digits_low[ ((num & ((0xF0000000) >> (i * 4))) >> (28 - (i * 4)))]);
            }
        }
    }
    if(!printing){
        printf("0");
    }
}

void print_dec(unsigned int num, bool sign){
    if(num == 0){
        putchar('0');
        return;
    }
    // most possible digits
    bool negative = false;
    if(sign & ((num >> 31) == 1)){
        num = ~num + 1;
        negative = true;
    }

    unsigned char buffer[20];
    int i = 0;
    while (num)
    {
        buffer[i++] = '0' + (num%10);
        num = num/10;
    }

    // add negative sign
    if(negative){
          buffer[i++] = '-';
    }

    while (i > 0)
    {
        putchar(buffer[--i]);
    }
    
    
}

int printf(const char* restrict format, ...) {
#if defined(__is_libk)
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
                case 'X':
                    print_hex(va_arg(parameters, unsigned int), true);
                    format++;
                    break;
                case 'x':
                    print_hex(va_arg(parameters, unsigned int), false);
                    format++;
                    break;
                case 'p':
                    print_hex((int)va_arg(parameters, void*), false);
                    format++;
                    break;
                case 'i':
                case 'd':
                case 'u':
                    print_dec(va_arg(parameters, unsigned int), !(*format == 'u'));
                    format++;
                    break;
                
                case '%':
                    putchar('%');
                    format++;
                    break;
                
                
            }
        }else{
            putchar(*format);
            format++;
            chCount++;

        }

    }

    va_end(parameters);
    return chCount;
#else
    va_list parameters;
	va_start(parameters, format);

    int res = vfprintf(stdout, format, parameters);
    va_end(parameters);
    return res;
#endif
}
