#include <kernel/pic.h>
#include <kernel/inline.h>
#include <kernel/idt.h>
#include <kernel/pit.h>
#include <kernel/keyboard.h>
#include <stdio.h>
#include <kernel/ata.h>

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();
extern void irq128();

void *irq_routines[16] =
{
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

void install_handler(int n, void (*func)(struct regs *r)){
    irq_routines[n] = func;
}


void PIT_install(){
    PIT_config(1000);
    install_handler(0, PIT_handler);   
}

void keyboard_install(){
    install_handler(1, keyboard_handler);   
}


void ata_install(){
    ata_init();
    install_handler(0xE, ata_handler);   
}


void irq_install(){
    PIC_remap(32, 40);

    idt_set_gate(0x20, (unsigned)irq0, 0x08, 0x8E);
    idt_set_gate(0x21, (unsigned)irq1, 0x08, 0x8E);
    idt_set_gate(0x22, (unsigned)irq2, 0x08, 0x8E);
    idt_set_gate(0x23, (unsigned)irq3, 0x08, 0x8E);

    idt_set_gate(0x24, (unsigned)irq4, 0x08, 0x8E);
    idt_set_gate(0x25, (unsigned)irq5, 0x08, 0x8E);
    idt_set_gate(0x26, (unsigned)irq6, 0x08, 0x8E);
    idt_set_gate(0x27, (unsigned)irq7, 0x08, 0x8E);

    idt_set_gate(0x28, (unsigned)irq8, 0x08, 0x8E);
    idt_set_gate(0x29, (unsigned)irq9, 0x08, 0x8E);
    idt_set_gate(0x2A, (unsigned)irq10, 0x08, 0x8E);
    idt_set_gate(0x2B, (unsigned)irq11, 0x08, 0x8E);

    idt_set_gate(0x2C, (unsigned)irq12, 0x08, 0x8E);
    idt_set_gate(0x2D, (unsigned)irq13, 0x08, 0x8E);
    idt_set_gate(0x2E, (unsigned)irq14, 0x08, 0x8E);
    idt_set_gate(0x2F, (unsigned)irq15, 0x08, 0x8E);

    idt_set_gate(0x80, (unsigned)irq128, 0x08, 0xEE);

    PIT_install();
    //keyboard_install();
    ata_install();
}

char l[30] = "0123456789ABCDEF";

void irq_handler(struct regs *r)
{
    void (*handler)(struct regs *r);
    handler = irq_routines[(r->int_no - 0x20)];
    if(handler){
        handler(r);

    }else{
        printf("INTERRUPTS %c", l[(r->int_no - 0x20)]);
    }
    PIC_sendEOI((r->int_no - 0x20));
}
