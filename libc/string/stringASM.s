.global asm_bulk_copy
asm_bulk_copy:
    push %ebp
    mov %esp, %ebp
    
    mov %edi, %eax
    mov 8(%ebp), %edx
    mov 12(%ebp), %ecx
    # Function body goes here

    # rep movsd
    pop %ebp
    hlt
    ret