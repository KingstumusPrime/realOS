#include <ui.h>
#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, char** argv, char** envp){
    stdout = fopen("/dev/tty", "w");
    fputc('!', stdout);

    while(1){
        for(;;){
        };
    };
    return 0;
}