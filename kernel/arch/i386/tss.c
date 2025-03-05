#include <kernel/tss.h>
#include <kernel/inline.h>
#include <kernel/gdt.h>

#include <stdio.h>
#include <string.h>

extern stack_top;


tss_entry_t tss_entry;

void tss_install(){
    gdt_set_gate(5, &tss_entry,sizeof(tss_entry) - 1, 0xE9, 0x0);

    memset(&tss_entry, 0, sizeof(tss_entry));

    tss_entry.ss0 = 0x10;
    tss_entry.esp0 = &stack_top;
    
}

void set_kernel_stack(unsigned int stack) { // Used when an interrupt occurs
	//printf("STACK %X", stack);
    tss_entry.esp0 = stack;
}