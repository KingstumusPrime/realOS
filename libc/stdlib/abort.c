#include <stdio.h>
#include <stdlib.h>


__attribute__((__noreturn__))

void abort(void){
#if defined(__is_libk)
    // kernel aborts
    printf("kernel error: ABORT!...");
    asm volatile("hlt");
#else
    // process aborts make it actually do something to end the process
    printf("ABORT!...");
#endif
    while (1){};
	__builtin_unreachable();
    
}
