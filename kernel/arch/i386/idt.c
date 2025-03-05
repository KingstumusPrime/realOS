#include <stdio.h>
#include <stdint.h>
#include <kernel/idt.h>
#include <string.h>
#include <kernel/isrs.h>
#include <kernel/inline.h>
#include <kernel/irq.h>
#include <kernel/pic.h>





/* Defines an IDT entry */
typedef struct
{
    uint16_t base_lo;
    uint16_t sel;        /* Our kernel segment goes here! */
    uint8_t always0;     /* This will ALWAYS be set to 0! */
    uint8_t flags;       /* Set using the above table! */
    uint16_t base_hi;
} __attribute__((packed)) idt_entry;

typedef struct
{
    uint16_t limit;
    idt_entry* base;
} __attribute__((packed)) idt_ptr;

/* Declare an IDT of 256 entries. Although we will only use the
*  first 32 entries in this tutorial, the rest exists as a bit
*  of a trap. If any undefined IDT entry is hit, it normally
*  will cause an "Unhandled Interrupt" exception. Any descriptor
*  for which the 'presence' bit is cleared (0) will generate an
*  "Unhandled Interrupt" exception */
idt_entry idt[256];
idt_ptr idtp;

/* This exists in 'start.asm', and is used to load our IDT */
extern void idt_load();

/* Use this function to set an entry in the IDT. Alot simpler
*  than twiddling with the GDT ;) */
void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags)
{
    /* We'll leave you to try and code this function: take the
    *  argument 'base' and split it up into a high and low 16-bits,
    *  storing them in idt[num].base_hi and base_lo. The rest of the
    *  fields that you must set in idt[num] are fairly self-
    *  explanatory when it comes to setup */

    idt[num].base_lo = (unsigned short) ((base & 0xFFFF));
    idt[num].base_hi = (unsigned short) ((base >> 16) & 0xFFFF);

    idt[num].always0 = 0;

    idt[num].sel = sel;
    idt[num].flags = flags;
}




/* Installs the IDT */
void idt_install()
{
    /* Sets the special IDT pointer up, just like in 'gdt.c' */
    idtp.limit = sizeof(idt) - 1;
    idtp.base = idt;

    /* Clear out the entire IDT, initializing it to zeros */
    memset(&idt, 0, sizeof(idt));

    /* Add any new ISRs to the IDT here using idt_set_gate */
    isrs_install();
    irq_install();
    IRQ_clear_mask(0);
    IRQ_clear_mask(1);

    /* Points the processor's internal register to the new IDT */
    idt_load();

}
