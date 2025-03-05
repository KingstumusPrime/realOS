#include <strings.h>
#include <ctype.h>

char* strdup(const char* s){
    int l = strlen(s);
    // plus one so there room for the null char
    char* dup = malloc(l + 1);
    for(int i = 0; i <= l; i++){
        dup[i] = s[i];
    }
    return dup;
}

int strcasecmp(const char* a, const char* b) {
    while (*a != '\0')
    {
        if(tolower(*a) != tolower(*b)){
            return tolower(*a) - tolower(*b);
        }
        a++;
        b++;
    }
    if(tolower(*a) == tolower(*b)){
        return 0;
    }else{
        return tolower(*a) - tolower(*b);
    }
    
}

int strncasecmp(const char* a, const char* b, size_t n) {
    while (n > 0 && *a && *b)
    {
        if(tolower(*a) != tolower(*b)){
            return tolower(*a) - tolower(*b);
        }
        a++;
        b++;
        n--;
    }
    if(n == 0){
        return 0;
    }
    if(tolower(*a) == tolower(*b)){
        return 0;
    }else{
        return tolower(*a) - tolower(*b);
    }
    
}