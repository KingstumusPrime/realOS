#include <stdio.h>
#include <string.h>
 
#if defined(__is_libk)
#include <kernel/syscalls/sysfile.h>
#endif


// this includes the main stdio function
// vfprintf vfscanf
// the others will be implemented in wrapper.c
#include <stdio.h>
#include <string.h>

static bool fprint(FILE* restrict stream, const char* data, size_t length) {
    // returns false on eof
	for(size_t i = 0; i < length; i++){
	    if(fputc(data[i], stream) == EOF){
	        return false;
	    };
	}
	return true;
}



void fprint_hex(FILE* restrict stream, unsigned int num, bool upper){
    bool printing = false;
    for(int i = 0; i < 8; i++){
        if(!printing){
            if( (num & ((0xF0000000) >> (i * 4))) > 0){
                printing = true;
            }
        }
        if(printing){
            if(upper){
               fputc(hex_digits[ ((num & ((0xF0000000) >> (i * 4))) >> (28 - (i * 4)))], stream);
            }else{
                fputc(hex_digits_low[ ((num & ((0xF0000000) >> (i * 4))) >> (28 - (i * 4)))], stream);
            }
        }
    }
    if(!printing){
        fputc('0', stream);
    }
}

void fprint_dec(FILE* restrict stream, unsigned int num, bool sign, int len){
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
    if(len > 0){
        buffer[i++] = '0';
    }

    // add negative sign
    if(negative){
          buffer[i++] = '-';
    }

    while (i > 0)
    {
        fputc(buffer[--i], stream);
    }
    
    
}

int vfprintf(FILE* restrict stream, const char* restrict format, va_list parameters) {
	int chCount = 1;
    while (*format != '\0')
    {
        if(*format == '%'){
            format++;
            switch (*format)
            {
            case'c':
                fputc(va_arg(parameters, int), stream);
                break;
            case 's':
                const char* str = va_arg(parameters, const char*);
                fprint(stream, str, strlen(str));
                break;
            case 'X':
            case 'x':
                fprint_hex(stream, va_arg(parameters, unsigned int), *format == 'X');
                break;
            case 'p':
                print_hex((int)va_arg(parameters, void*), false);
                break;
            case 'i':
            case 'd':
            case 'u':
                fprint_dec(stream, va_arg(parameters, unsigned int),  !(*format == 'u'), 0);
                break;
            case '.':
                format++;
                unsigned int width;
                format += scanf_int(format, &width);
                if(*format == 'd'){
                format++;
                fprint_dec(stream, va_arg(parameters, unsigned int),  true, width);
                }
            default:
                break;
            }
            format++;
        }else{
            fputc(*format, stream);

            format++;
            chCount++;
        }

    }
    
    return chCount;
}

