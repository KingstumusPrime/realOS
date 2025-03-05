################################################################
#
# $Id:$
#
# $Log:$
#

ifeq ($(V),1)
	VB=''
else
	VB=@
endif


CC=i686-myos-gcc
CFLAGS+=-ggdb3 -Os -I./include
LDFLAGS+=-Wl,--gc-sections
CFLAGS+=-ggdb3 -Wall -DNORMALUNIX -DLINUX -DSNDSERV -D_DEFAULT_SOURCE # -DUSEASM
# ../../realOS/sysroot/usr/lib/crt0.o  ../../realOS/sysroot/usr/lib/crti.o  ../../realOS/sysroot/usr/lib/crtn.o 
# LIBS+=../../realOS/sysroot/usr/lib/crt0.o  ../../realOS/sysroot/usr/lib/crti.o  ../../realOS/sysroot/usr/lib/crtn.o ../../realOS/sysroot/usr/lib/libc.a

# subdirectory for objects
OBJDIR=build
OUTPUT=top.elf

SRC_DOOM = main.o
OBJS += $(addprefix $(OBJDIR)/, $(SRC_DOOM))

all:	 $(OUTPUT)

clean:
	rm -rf $(OBJDIR)
	rm -f $(OUTPUT)
	rm -f $(OUTPUT).gdb
	rm -f $(OUTPUT).map

$(OUTPUT):	$(OBJS)
	@echo [Linking $@]
	$(VB)$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) \
	-o $(OUTPUT) $(LIBS) -Wl,-Map,$(OUTPUT).map
	@echo [Size]
	-$(CROSS_COMPILE)size $(OUTPUT)

$(OBJS): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o:	%.c
	@echo [Compiling $<]
	$(VB)$(CC) $(CFLAGS) -c $< -o $@

print:
	@echo OBJS: $(OBJS)

