
#include <string.h>
char* strchr(const char* s, int c){
    int l = strlen(s);
    int v = 0;
    for(int i = 0; i < l; i++){
        if(*s == c){
            v = *s;
            return v;
        }
    }
    return v;
}