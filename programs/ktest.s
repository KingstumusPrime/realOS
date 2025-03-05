	.file	"ktest.c"
	.text
	.section	.rodata
	.align 4
	.type	hex_digits, @object
	.size	hex_digits, 30
hex_digits:
	.string	"0123456789ABCDEF"
	.zero	13
	.align 4
	.type	hex_digits_low, @object
	.size	hex_digits_low, 30
hex_digits_low:
	.string	"0123456789abcdef"
	.zero	13
.LC0:
	.string	"w"
.LC1:
	.string	"/dev/tty"
.LC2:
	.string	"HELLO"
.LC3:
	.string	" WORLD\n"
	.text
	.globl	main
	.type	main, @function
main:
	leal	4(%esp), %ecx
	andl	$-16, %esp
	pushl	-4(%ecx)
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ecx
	subl	$4, %esp
	subl	$8, %esp
	pushl	$.LC0
	pushl	$.LC1
	call	fopen
	addl	$16, %esp
	movl	%eax, stdout
	subl	$12, %esp
	pushl	$.LC2
	call	printf
	addl	$16, %esp
	movl	stdout, %eax
	subl	$12, %esp
	pushl	%eax
	call	fflush
	addl	$16, %esp
	subl	$12, %esp
	pushl	$.LC3
	call	puts
	addl	$16, %esp
	movl	stdout, %eax
	subl	$12, %esp
	pushl	%eax
	call	fflush
	addl	$16, %esp
.L2:
	jmp	.L2
	.size	main, .-main
	.ident	"GCC: (GNU) 12.2.0"
