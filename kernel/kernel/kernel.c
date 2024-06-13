#include <stdio.h>
#include <string.h>

#include <kernel/tty.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>

void kernel_main(void) {
	gdt_install();
	idt_install();
	terminal_initialize();
	char x = 'x';
	unsigned char* s = "my char x =";

	printf("Hello, kernel World! %s %c", s, x);
}