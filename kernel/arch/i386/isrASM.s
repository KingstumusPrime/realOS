
# ISR
.global isr0
.global isr1
.global isr2
.global isr3
.global isr4
.global isr5
.global isr6
.global isr7
.global isr8
.global isr9
.global isr10
.global isr11
.global isr12
.global isr13
.global isr14
.global isr15
.global isr16
.global isr17
.global isr18
.global isr19
.global isr20
.global isr21
.global isr22
.global isr23
.global isr24
.global isr25
.global isr26
.global isr27
.global isr28
.global isr29
.global isr30
.global isr31

isr0:
    cli
    push $0x00    # A normal ISR stub that pops a dummy error code to keep a
                   # uniform stack frame
    push $0x00
    jmp isr_common_stub

isr1:
    cli
    push $0x00    # A normal ISR stub that pops a dummy error code to keep a
                   # uniform stack frame
    push $0x01
    jmp isr_common_stub

isr2:
    cli
    push $0x00    # A normal ISR stub that pops a dummy error code to keep a
                   # uniform stack frame
    push $0x02
    jmp isr_common_stub
isr3:
    cli
    push $0x00    # A normal ISR stub that pops a dummy error code to keep a
                   # uniform stack frame
    push $0x03
    jmp isr_common_stub

isr4:
    cli
    push $0x00    # A normal ISR stub that pops a dummy error code to keep a
                   # uniform stack frame
    push $0x04
    jmp isr_common_stub

isr5:
    cli
    push $0x00    # A normal ISR stub that pops a dummy error code to keep a
                   # uniform stack frame
    push $0x05
    jmp isr_common_stub

isr6:
    cli
    push $0x00    # A normal ISR stub that pops a dummy error code to keep a
                   # uniform stack frame
    push $0x06
    jmp isr_common_stub

isr7:
    cli
    push $0x00    # A normal ISR stub that pops a dummy error code to keep a
                   # uniform stack frame
    push $0x07
    jmp isr_common_stub

isr8:
    cli
    push $0x08    # real error code
    jmp isr_common_stub

isr9:
    cli
    push $0x00    # A normal ISR stub that pops a dummy error code to keep a
                   # uniform stack frame
    push $0x09
    jmp isr_common_stub

isr10:
    cli
    push $0x0A    # real error code
    jmp isr_common_stub
isr11:
    cli
    push $0x0B    # real error code
    jmp isr_common_stub

isr12:
    cli
    push $0x0C    # real error code
    jmp isr_common_stub

isr13:
    cli
    push $0x0D    # real error code
    jmp isr_common_stub

isr14:
    cli
    push $0x0E    # real error code
    jmp isr_common_stub

isr15:
    cli
    push $0x00    # A normal ISR stub that pops a dummy error code to keep a
                   # uniform stack frame
    push $0x0F
    jmp isr_common_stub

isr16:
    cli
    push $0x00    # A normal ISR stub that pops a dummy error code to keep a
                   # uniform stack frame
    push $0x10
    jmp isr_common_stub

isr17:
    cli
    # push $0x00     A normal ISR stub that pops a dummy error code to keep a
    #                 uniform stack frame
    push $0x11
    jmp isr_common_stub

isr18:
    cli
    push $0x00    # A normal ISR stub that pops a dummy error code to keep a
                   # uniform stack frame
    push $0x12
    jmp isr_common_stub

isr19:
    cli
    push $0x00    # A normal ISR stub that pops a dummy error code to keep a
                   # uniform stack frame
    push $0x13
    jmp isr_common_stub

isr20:
    cli
    push $0x00    # A normal ISR stub that pops a dummy error code to keep a
                   # uniform stack frame
    push $0x14
    jmp isr_common_stub

isr21:
    cli
    # push $0x00    A normal ISR stub that pops a dummy error code to keep a
                   # uniform stack frame
    push $0x15
    jmp isr_common_stub

isr22:
    cli
    push $0x00    # A normal ISR stub that pops a dummy error code to keep a
                   # uniform stack frame
    push $0x16
    jmp isr_common_stub

isr23:
    cli
    push $0x00    # A normal ISR stub that pops a dummy error code to keep a
                   # uniform stack frame
    push $0x17
    jmp isr_common_stub

isr24:
    cli
    push $0x00    # A normal ISR stub that pops a dummy error code to keep a
                   # uniform stack frame
    push $0x18
    jmp isr_common_stub

isr25:
    cli
    push $0x00    # A normal ISR stub that pops a dummy error code to keep a
                   # uniform stack frame
    push $0x19
    jmp isr_common_stub

isr26:
    cli
    push $0x00    # A normal ISR stub that pops a dummy error code to keep a
                   # uniform stack frame
    push $0x1A
    jmp isr_common_stub

isr27:
    cli
    push $0x00    # A normal ISR stub that pops a dummy error code to keep a
                   # uniform stack frame
    push $0x1B
    jmp isr_common_stub

isr28:
    cli
    push $0x00    # A normal ISR stub that pops a dummy error code to keep a
                   # uniform stack frame
    push $0x1C
    jmp isr_common_stub

isr29:
    cli
    push $0x00    # A normal ISR stub that pops a dummy error code to keep a
                   # uniform stack frame
    push $0x1D
    jmp isr_common_stub

isr30:
    cli
    push $0x00    # A normal ISR stub that pops a dummy error code to keep a
                   # uniform stack frame
    push $0x1E
    jmp isr_common_stub

isr31:
    cli
    push $0x00    # A normal ISR stub that pops a dummy error code to keep a
                   # uniform stack frame
    push $0x1F
    jmp isr_common_stub


.extern fault_handler
isr_common_stub:
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
    mov $fault_handler, %eax
    call %eax       # A special call, preserves the 'eip' register
    pop %eax
    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa
    add $8, %esp     # Cleans up the pushed error code and pushed ISR number
    iret           # pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP!