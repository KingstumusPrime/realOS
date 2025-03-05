#ifndef _KERNEL_INLINE_H
#define _KERNEL_INLINE_H
 
#include <stddef.h>
#include <stdint.h>

extern void clearInterrupts();
extern void setInterrupts();

static inline void outb(uint16_t port, uint8_t val)
{
   __asm__ volatile ( "outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
    /* There's an outb %al, $imm8 encoding, for compile-time constant port numbers that fit in 8b. (N constraint).
     * Wider immediate constants would be truncated at assemble-time (e.g. "i" constraint).
     * The  outb  %al, %dx  encoding is the only option for all other cases.
     * %1 expands to %dx because  port  is a uint16_t.  %w1 could be used if we had the port number a wider C type */
}

static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    __asm__ volatile ( "inb %w1, %b0"
                   : "=a"(ret)
                   : "Nd"(port)
                   : "memory");
    return ret;
}

static inline void outsw(uint16_t port, uint16_t val)
{
	__asm__ __volatile__("outw %0,%1"
			:
			: "a"(val), "dN"(port));
}

static inline uint16_t insw(uint16_t port)
{
	uint16_t val;
	__asm__ __volatile__("inw %1,%0"
			: "=a"(val)
			: "dN"(port));
	return val;
}

void io_wait(void);

static inline void outl(uint16_t port, uint32_t val)
{
   __asm__ volatile ( "outl %%eax, %%dx" : : "d"(port), "a"(val));
    /* There's an outb %al, $imm8 encoding, for compile-time constant port numbers that fit in 8b. (N constraint).
     * Wider immediate constants would be truncated at assemble-time (e.g. "i" constraint).
     * The  outb  %al, %dx  encoding is the only option for all other cases.
     * %1 expands to %dx because  port  is a uint16_t.  %w1 could be used if we had the port number a wider C type */
}


static inline uint32_t inl(uint16_t port)
{
	uint32_t val;
	__asm__ __volatile__("inl %%dx,%%eax"
			: "=a"(val)
			: "d"(port));
	return val;
}


struct regs
{
    unsigned int gs, fs, es, ds;      /* pushed the segs last */
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    unsigned int int_no, err_code;    /* our 'push byte #' and ecodes do this */
    unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */ 
};
#endif