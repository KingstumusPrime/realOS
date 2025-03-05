#ifndef _KERNEL_TSS_H
#define _KERNEL_TSS_H

typedef struct
{
	unsigned long prev_tss; // The previous TSS - with hardware task switching these form a kind of backward linked list.
	unsigned long esp0;     // The stack pointer to load when changing to kernel mode.
	unsigned long ss0;      // The stack segment to load when changing to kernel mode.
	// Everything below here is unused.
	unsigned long esp1; // esp and ss 1 and 2 would be used when switching to rings 1 or 2.
	unsigned long ss1;
	unsigned long esp2;
	unsigned long ss2;
	unsigned long cr3;
	unsigned long eip;
	unsigned long eflags;
	unsigned long eax;
	unsigned long ecx;
	unsigned long edx;
	unsigned long ebx;
	unsigned long esp;
	unsigned long ebp;
	unsigned long esi;
	unsigned long edi;
	unsigned long es;
	unsigned long cs;
	unsigned long ss;
	unsigned long ds;
	unsigned long fs;
	unsigned long gs;
	unsigned long ldt;
	unsigned short trap;
	unsigned short iomap_base;
} __attribute__((__packed__)) tss_entry_t;
void tss_install();
void set_kernel_stack(unsigned int stack);
#endif