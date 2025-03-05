#include <stdio.h>
#include <string.h>


int fprintf(FILE* restrict stream, const char* restrict format, ...) {
	va_list parameters;
	va_start(parameters, format);

    int res = vfprintf(stream, format, parameters);
    va_end(parameters);
    return res;
}
