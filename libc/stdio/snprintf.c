#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>

int snprint_dec(char* buf, int start, int max, unsigned int num, bool sign, int len){
    if(max <= 0){
        return 0;
    }
    if(num == 0 && max > 1){
        buf[start] = '0';
        return 1;
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
        len--;
    }
    if(len > 0){
        buffer[i++] = '0';
    }

    // add negative sign
    if(negative){
          buffer[i++] = '-';
    }

    int p = 0;
    int ret = i;
    while (i > 0)
    {
        if(p < max){
            buf[start + p] = buffer[--i];
            p++;    
        }else{
            i--;
        }
    }
    return ret;
    
}



int snprint_hex(char* buf, int start, int max, unsigned int num, bool upper){
    if(max ==0){
        return 0;
    }
    bool printing = false;
    int p = 0;
    for(int i = 0; i < 8; i++){
        if(!printing){
            if( (num & ((0xF0000000) >> (i * 4))) > 0){
                printing = true;
            }
        }
        if(printing){
            if(p < max){
                if(upper){
                    buf[start + p] = ("%c", hex_digits[ ((num & ((0xF0000000) >> (i * 4))) >> (28 - (i * 4)))]);
                }else{
                    buf[start + p] = ("%c", hex_digits_low[ ((num & ((0xF0000000) >> (i * 4))) >> (28 - (i * 4)))]);
                }
            }

            p++;

        }
    }
    if(!printing){
        printf("0");
        return 1;
    }
    return p;
}

int sn_print(char* buf, int start, int max, const char* data){
    int j = 0;
    int i = start;
    while (true){
        if(i < start + max){
            buf[i] = data[j];
            i++;   
        }

        j++;
        if(data[j] == '\0'){
            if(j == max){
                return j + 1;
            }else{
                return j;
            }
	    }
    }

    return max;
}

int vsnprintf(char* restrict s, size_t n, const char* restrict format, va_list arg){
    int chCount = 0;
    while (*format != '\0')
    {
        if(*format == '%'){
            format++;
            switch (*format){
                case'c':
                    s[chCount] = va_arg(arg, int);
                    format++;
                    break;
                case 's':
                    const char* str = va_arg(arg, const char*);
                    int l = sn_print(s, chCount, n - chCount, str);
                    format++;
                    chCount += l;


                    break;
                case 'X':
                    chCount += snprint_hex(s, chCount, n - chCount, va_arg(arg, unsigned int), true);
                    format++;
                    break;
                case 'x':
                    chCount += snprint_hex(s, chCount, n - chCount,va_arg(arg, unsigned int), false);
                    format++;
                    break;
                case 'i':
                case 'd':
                case 'u':
                    chCount += snprint_dec(s, chCount, n - chCount, va_arg(arg, unsigned int), !(*format == 'u'), 0);
                    format++;
                    break;
                case '.':
                    format++;
                    unsigned int width;
                    format += scanf_int(format, &width);
                    if(*format == 'd'){
                    format++;
                    chCount += snprint_dec(s, chCount, n - chCount, va_arg(arg, unsigned int), true, width);    
                    }
                    break;
                case '%':
                    s[chCount] = '%';
                    format++;
                    break;
                
                
            }
        }else{
            if(chCount < n - 1){
                s[chCount] = *format;
            }

            format++;
            chCount++;

        }
        
   
    }

    if(chCount >= n - 1){
        s[n - 1] = '\0';
    }else{
        s[chCount] = '\0';
    }
    return chCount;
}

int snprintf(char* restrict s, size_t n, const char* format, ...){
    va_list parameters;
	va_start(parameters, format);
	int res = vsnprintf(s, n, format, parameters);
    va_end(parameters);
    return res;
}
