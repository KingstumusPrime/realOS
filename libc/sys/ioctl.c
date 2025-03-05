#include <stdarg.h>

#if defined(__is_libk)
#include <kernel/syscalls/sysfile.h>
#else
#include <syscalls.h>
#endif

void ioctl(int fd, int func, ...){
	va_list args;
  	va_start(args, func);
	int d = args;
    #if defined(__is_libk)
	kioctl(fd, func, d);
    #else
    syscall_4(17, fd, func, d);
    #endif

	va_end(args);
}