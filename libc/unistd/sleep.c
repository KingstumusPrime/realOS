// BUGGED DONT USE
#include <unistd.h>

#if defined(__is_libk)
#include <kernel/pit.h>
#include <stdio.h>
#endif
 
unsigned int sleep(unsigned int seconds){

#if defined(__is_libk)

    unsigned int target;
    target = ticks + 0;
    while (ticks <= target);
    printf("SLEEP");
    return 0;

#else
    printf("NO SYS CALL VERSION");
	// TODO: Implement stdio and the write system call.
#endif
}