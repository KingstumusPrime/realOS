DEFAULT_HOST!=../default-host.sh
HOST?=DEFAULT_HOST
HOSTARCH!=../target-triplet-to-arch.sh $(HOST)
 
CFLAGS?=-O2 -g
CPPFLAGS?=
LDFLAGS?=
LIBS?=
 
DESTDIR?=
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
INCLUDEDIR?=$(PREFIX)/include
LIBDIR?=$(EXEC_PREFIX)/lib
 
CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra
CPPFLAGS:=$(CPPFLAGS) -D__is_libc -Iinclude
LIBK_CFLAGS:=$(CFLAGS)
LIBK_CPPFLAGS:=$(CPPFLAGS) -D__is_libk
 
ARCHDIR=arch/$(HOSTARCH)
 
include $(ARCHDIR)/make.config
 
CFLAGS:=$(CFLAGS) $(ARCH_CFLAGS)
CPPFLAGS:=$(CPPFLAGS) $(ARCH_CPPFLAGS)
LIBK_CFLAGS:=$(LIBK_CFLAGS) $(KERNEL_ARCH_CFLAGS)
LIBK_CPPFLAGS:=$(LIBK_CPPFLAGS) $(KERNEL_ARCH_CPPFLAGS)
 
FREEOBJS=\
$(ARCH_FREEOBJS) \
errno/errno.o \
init/__init_libc.o \
syscalls/syscall_helpers.o \
syscalls/syscalls.o \
math/abs.o \
stdio/printf.o \
stdio/putchar.o \
stdio/puts.o \
stdio/fputs.o \
stdio/snprintf.o \
string/stringASM.o \
string/memcmp.o \
string/memcpy.o \
string/memmove.o \
string/memset.o \
string/strlen.o \
string/malloc.o \
string/strcmp.o \
string/strncmp.o \
string/strncpy.o \
string/strchr.o \
string/strstr.o \
string/strrchr.o \
strings/strings.o \
stdlib/abort.o \
stdlib/exit.o \
stdlib/atoi.o \
stdlib/atof.o \
stdlib/calloc.o \
stdlib/realloc.o \
ctype/tolower.o \
ctype/toupper.o \
ctype/isspace.o \
unistd/sleep.o \
stdio/init.o \
stdio/compound.o \
stdio/sscanf.o \
stdio/misc.o \
stdio/formatted.o \
stdio/wrapper.o \
system/system.o \
sys/ioctl.o \
ui/ui.o \
termios/termios.o \



HOSTEDOBJS=\
$(ARCH_HOSTEDOBJS) \

OBJS=\
$(FREEOBJS) \
$(HOSTEDOBJS) \
 
LIBK_OBJS=$(FREEOBJS:.o=.libk.o)
 
BINARIES= libc.a libk.a $(ARCHDIR)/crti.o $(ARCHDIR)/crt0.o  $(ARCHDIR)/crtn.o # Not ready for libc yet.
#BINARIES=libk.a
 
.PHONY: all clean install install-headers install-libs
.SUFFIXES: .o .libk.o .c .S
 
all: $(BINARIES)
 
libc.a: $(OBJS)
	$(AR) rcs $@ $(OBJS)
 
libk.a: $(LIBK_OBJS)
	$(AR) rcs $@ $(LIBK_OBJS)
 
.c.o:
	$(CC) -MD -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)
 
.s.o:
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)
 
.c.libk.o:
	$(CC) -MD -c $< -o $@ -std=gnu11 $(LIBK_CFLAGS) $(LIBK_CPPFLAGS)
 
.s.libk.o:
	$(CC) -MD -c $< -o $@ $(LIBK_CFLAGS) $(LIBK_CPPFLAGS)
 
clean:
	rm -f $(BINARIES) *.a
	rm -f $(OBJS) $(LIBK_OBJS) *.o */*.o */*/*.o
	rm -f $(OBJS:.o=.d) $(LIBK_OBJS:.o=.d) *.d */*.d */*/*.d
 
install: install-headers install-libs
 
install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.
 
install-libs: $(BINARIES)
	mkdir -p $(DESTDIR)$(LIBDIR)
	cp $(BINARIES) $(DESTDIR)$(LIBDIR)
 
-include $(OBJS:.o=.d)
-include $(LIBK_OBJS:.o=.d)