#include <kernel/pit.h>
#include <stdio.h>
#include <kernel/multitasking/task_state.h>

unsigned long long ticks = 0;
void PIT_handler(struct regs *r){
    ticks++;

}

void PIT_config(int hz){
    int divisor = 1193180 / hz;       /* Calculate our divisor */
    outb(0x43, 0x36);             /* Set our command byte 0x36 */
    outb(0x40, divisor & 0xFF);   /* Set low byte of divisor */
    outb(0x40, divisor >> 8);     /* Set high byte of divisor */
}

unsigned read_pit_count(void) {
	unsigned count = 0;
	
	// Disable interrupts
	__asm__ volatile ("cli");
	
	// al = channel in bits 6 and 7, remaining bits clear
	outb(0x43,0b0000000);
	
	count = inb(0x40);		// Low byte
	count |= inb(0x40)<<8;		// High byte
	
	return count;
}