#ifndef _KERNEL_PIC_H
#define _KERNEL_PIC_H
 
#include <stddef.h>
#include <kernel/inline.h>

#define PIC1  0x0020
#define PIC2  0x00A0
#define PIC1_DATA  0x21
#define PIC2_DATA  0xA1

#define ICW1_ICW4	0x01		/* Indicates that ICW4 will be present */
#define ICW1_SINGLE	0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	0x10		/* Initialization - required! */
 
#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	0x10		/* Special fully nested (not) */

#define PIC_EOI  0x20

void PIC_sendEOI(uint8_t irq);

void PIC_remap(int offset1, int offset2);


void IRQ_set_mask(uint8_t IRQline);
void IRQ_clear_mask(uint8_t IRQline);
#endif