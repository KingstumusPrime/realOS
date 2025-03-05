
#include <stdio.h>
#include <string.h>

int fputs(const char *str, FILE *stream){
    int i = 0;
    for(i = 0; i < strlen(str); i++){
        if(fputc(str[i], stream) == EOF){
            return i;
        }
    }
    return i;
}