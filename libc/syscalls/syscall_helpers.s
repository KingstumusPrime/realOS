.macro save_state
    push %esp
    push %ebx
    push %ecx
    push %edx
    push %esi
    push %edi
    push %ebp
.endm

.macro restore_state
    pop %ebp
    pop %edi
    pop %esi
    pop %edx
    pop %ecx
    pop %ebx
    pop %esp
.endm


.global syscall_1
.extern syscall_1
.type syscall_1, @function
syscall_1:

    save_state

    mov 32(%esp), %eax

    int $0x80

    restore_state
    ret

.global syscall_2
.extern syscall_2
.type syscall_2, @function
syscall_2:

    save_state

    mov 32(%esp), %eax
    mov 36(%esp), %ebx

    int $0x80

    restore_state
    ret


.global syscall_3
.extern syscall_3
.type syscall_3, @function
syscall_3:

    save_state

    mov 32(%esp), %eax
    mov 36(%esp), %ebx
    mov 40(%esp), %ecx

    int $0x80

    restore_state
    ret

.global syscall_4
.extern syscall_4
.type syscall_4, @function
syscall_4:
    save_state

    mov 32(%esp), %eax
    mov 36(%esp), %ebx
    mov 40(%esp), %ecx
    mov 44(%esp), %edx

    int $0x80

    restore_state
    ret


.global mov_eax
.extern mov_eax
.type mov_eax, @function
mov_eax:
    push %edi                 # Save edi
    movl 8(%esp), %edi        # Adjust for the pushed value
    mov %edi, %eax
    pop %edi                  # Restore edi
    ret


.global mov_ebx
.extern mov_ebx
mov_ebx:
	movl 4(%esp), %ebx
    ret

.global mov_ecx
.extern mov_ecx
mov_ecx:
	movl 4(%esp), %ecx

    ret

.global mov_edx
.extern mov_edx
mov_edx:
	movl 4(%esp), %edx
    ret

.global call_int
.extern call_int
call_int:
    int $0x80
    ret

.global get_esp
.extern get_esp
get_esp:
	movl %esp, %eax
    ret

