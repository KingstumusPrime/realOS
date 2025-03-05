.section .text
.global _start
_start:
    mov $1, %eax
    mov $0x80, %ebx
    int $0x80
loop:
    jmp loop

.section .data
dummy:  .asciz "Hello, World!\n"
