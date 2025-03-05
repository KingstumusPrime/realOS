.global mov_eax
.extern mov_eax
mov_eax:
	movl 4(%esp), %edi
    mov %edi, %eax
    ret

.global mov_ebx
.extern mov_ebx
mov_ebx:
	movl 4(%esp), %edi
    mov %edi, %ebx
    ret

.global get_eax
.extern get_eax
get_eax:
	movl %eax, %eax
    ret

.global get_esp
.extern get_esp
get_esp:
	movl %esp, %eax

    ret