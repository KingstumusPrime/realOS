#include <strings.h>

int isspace(int c){
    switch (c)
    {
    case ' ':
    case '\n':
    case '\t':
    case '\v':
    case '\f':
    case '\r':
        return c;
        break;
    
    default:
        return 0;
        break;
    }
}