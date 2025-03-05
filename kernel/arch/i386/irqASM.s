# irq
.global irq0
.global irq1
.global irq2
.global irq3
.global irq4
.global irq5
.global irq6
.global irq7
.global irq8
.global irq9
.global irq10
.global irq11
.global irq12
.global irq13
.global irq14
.global irq15
.global irq128

irq0:
    cli
    push $0x00 

    push $0x20

    jmp irq_common_stub
irq1:
    cli
    push $0x00 

    push $0x21

    jmp irq_common_stub
irq2:
    cli
    push $0x00 

    push $0x22

    jmp irq_common_stub
irq3:
    cli
    push $0x00 

    push $0x23

    jmp irq_common_stub

irq4:
    cli
    push $0x00 

    push $0x24

    jmp irq_common_stub
irq5:
    cli
    push $0x00 

    push $0x25

    jmp irq_common_stub
irq6:
    cli
    push $0x00 

    push $0x26

    jmp irq_common_stub
irq7:
    cli
    push $0x00 

    push $0x27

    jmp irq_common_stub

irq8:
    cli
    push $0x00 

    push $0x28

    jmp irq_common_stub
irq9:
    cli
    push $0x00 

    push $0x29

    jmp irq_common_stub
irq10:
    cli
    push $0x00 

    push $0x2A

    jmp irq_common_stub
irq11:
    cli
    push $0x00 

    push $0x2B

    jmp irq_common_stub

irq12:
    cli
    push $0x00 

    push $0x2C

    jmp irq_common_stub
irq13:
    cli
    push $0x00 

    push $0x2D

    jmp irq_common_stub
irq14:
    cli
    push $0x00 

    push $0x2E

    jmp irq_common_stub
irq15:
    cli
    push $0x00 

    push $0x2F

    jmp irq_common_stub

irq128:
    cli
    jmp irq_sys
    

.extern syscall_c

irq_sys:

    # already on stack: ss, sp, flags, cs, ip.
    # need to push ax, gs, fs, es, ds, -ENOSYS, bp, di, si, dx, cx, and bx
    push %eax
    pushl %gs
    pushl %fs
    pushl %es
    pushl %ds
    pushl $0xdead
    push %ebp
    push %edi
    push %esi
    push %edx
    push %ecx
    push %ebx
    push %esp
    call syscall_c
    add $4, %esp

    pop %ebx
    pop %ecx
    pop %edx
    pop %esi
    pop %edi
    pop %ebp
    add $4, %esp
    pop %ds
    pop %es
    pop %fs
    pop %gs
    add $4, %esp

    iret



.extern irq_handler
irq_common_stub:
    pusha
    push %ds
    push %es
    push %fs
    push %gs
    mov $0x10, %ax   # Load the Kernel Data Segment descriptor!
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %esp, %eax    # Push us the stack
    push %eax
    mov $irq_handler, %eax
    call %eax       # A special call, preserves the 'eip' register
    pop %eax
    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa
    add $8, %esp     # Cleans up the pushed error code and pushed ISR number
    iret           # pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP!