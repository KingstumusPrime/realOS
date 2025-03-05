# Declare constants for the multiboot header.
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1     
.set GRAPHMODE, 1<< 2
.set MODE_TYPE, 0
.set WIDTH, 1024
.set HEIGHT, 768
.set DEPTH, 32


# random multiboot items
.set HEADER_ADDR, 0
.set LOAD_ADDR, 0
.set LOAD_END_ADDR, 0
.set BSS_END_ADDR, 0
.set ENTRY_ADDR, 0


# provide memory map
.set FLAGS,    ALIGN | MEMINFO | GRAPHMODE  # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

# Declare a multiboot header that marks the program as a kernel.
.section .multiboot.data, "aw"
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM
.long HEADER_ADDR
.long LOAD_ADDR
.long LOAD_ADDR
.long LOAD_END_ADDR
.long BSS_END_ADDR
.long ENTRY_ADDR
.long MODE_TYPE
.long WIDTH
.long HEIGHT
.long DEPTH
.skip 4 * 13 # space for the returned info

# Allocate the initial stack.
.section .bootstrap_stack, "aw", @nobits
stack_bottom:
.skip 16384 # 16 KiB

.global stack_top
stack_top:



# Preallocate pages used for paging. Don't hard-code addresses and assume they
# are available, as the bootloader might have loaded its multiboot structures or
# modules there. This lets the bootloader know it must avoid the addresses.

# 10 empty allocated pages this is the number of pages useable by the whole system for data
.set NUMPAGES, 10

.section .bss, "aw", @nobits
	.align 4096
.global boot_page_directory
boot_page_directory:
	.skip 4096
boot_page_table1:
	.skip 4096
boot_page_table2:
	.skip 4096
boot_page_table3:
	.skip 4096 * NUMPAGES
boot_page_graphics:
	.skip 4096

.section .data
# Mem lower : 0
# mem upper : 4
# fb_addr : 8
# fb_pitch : 12
# fb_width : 16
# fb_height : 20
# fb_bpp : 24
# fb_type 25
# fb_info 26-31
# SEE enable 31-35
boot_data:
	.skip 35

# Further page tables may be required if the kernel grows beyond 3 MiB.



# The kernel entry point.
.section .multiboot.text, "a"

.global _SSE_init
.type _SSE_init, @function
_SSE_init:


	mov $1, %eax
	cpuid
	test $1<<25, %edx
	jz .NoSSE

	# finish up
	mov %cr0, %eax
	and $0xFFFB, %ax # clear coprosser emulation
	or $0x22, %ax
	mov %eax, %cr0
	mov %cr4, %eax
	or $0x600, %ax # set random flags
	mov %eax, %cr4


	# enable flag
	movl $(boot_data - 0xC0000000), %esi
	addl $32, %esi
	movl $1, (%esi)

	ret

.NoSSE:
	movl $(boot_data - 0xC0000000), %esi
	addl $32, %esi
	movl $0, (%esi)

	ret




.global _multiboot_init
.type _multiboot_init, @function

_multiboot_init:
	# get mem bound
	movl $(boot_data - 0xC0000000), %esi
	addl $4, %ebx
	movl (%ebx), %eax
	movl %eax, (%esi)
	addl $4, %esi

	# mem upper
	addl $4, %ebx
	movl (%ebx), %eax
	movl %eax, (%esi)
	addl $4, %esi

	# fb_addr
	addl $80, %ebx
	movl (%ebx), %eax
	movl %eax, (%esi)
	addl $4, %esi

	# fb_pitch
	addl $8, %ebx
	movl (%ebx), %eax
	movl %eax, (%esi)
	addl $4, %esi

	# fb_width
	addl $4, %ebx
	movl (%ebx), %eax
	movl %eax, (%esi)
	addl $4, %esi

	# fb_height
	addl $4, %ebx
	movl (%ebx), %eax
	movl %eax, (%esi)
	addl $4, %esi

	# fb_bpp
	addl $4, %ebx
	movl (%ebx), %eax
	movl %eax, (%esi)
	addl $4, %esi


	# skip fb_type


	# fb_info
	addl $5, %ebx
	movl (%ebx), %eax
	movl %eax, (%esi)
	addl $4, %esi

	ret



.global _video_paging_init
.type _video_paging_init, @function
_video_paging_init:
	movl $(boot_page_graphics - 0xC0000000), %edi
	movl $1024, %ecx 
	
	# get the addr
	movl $(boot_data - 0xC0000000), %esi
	addl $8, %esi

	movl (%esi), %eax
1:
	movl %eax, %esi
	orl $0x03, %esi

	movl %esi, (%edi)

	addl $4096, %eax
	addl $4, %edi

	loop 1b

	ret

.global _start
.type _start, @function
_start:
	call _multiboot_init
	call _SSE_init


	# Physical address of boot_page_table1.
	# TODO: I recall seeing some assembly that used a macro to do the
	#       conversions to and from physical. Maybe this should be done in this
	#       code as well?
	movl $(boot_page_table1 - 0xC0000000), %edi
    movl $(boot_page_table2 - 0xC0000000), %ebx
	# First address to map is address 0.
	# TODO: Start at the first kernel page instead. Alternatively map the first
	#       1 MiB as it can be generally useful, and there's no need to
	#       specially map the VGA buffer.
	movl $0, %esi
    movl 0x400000, %eax
	# Map 1023 pages. The 1024th will be the VGA text buffer.
	movl $1024, %ecx

1:


	# Map physical address as "present, writable". Note that this maps
	# .text and .rodata as writable. Mind security and map them as non-writable.
	movl %esi, %edx
	orl $0x03, %edx
	movl %edx, (%edi)

    movl %eax, %edx
	orl $0x03, %edx
	movl %edx, (%ebx)

2:
	# Size of page is 4096 bytes.
	addl $4096, %esi
    addl $4096, %eax
	# Size of entries in boot_page_table1 is 4 bytes.
	addl $4, %edi
    addl $4, %ebx
	# Loop to the next entry if we haven't finished.
	loop 1b

3:
    # Map the last part of the first table
	# Map VGA video memory to 0xC03FF000 as "present, writable".
	# movl $(0x000B8000 | 0x003), boot_page_table1 - 0xC0000000 + 1023 * 4
	# 0x000B8000
	# movl $(0xFD150000 | 0x003), boot_page_table2 - 0xC0000000 + 1023 * 4


	# The page table is used at both page directory entry 0 (virtually from 0x0
	# to 0x3FFFFF) (thus identity mapping the kernel) and page directory entry
	# 768 (virtually from 0xC0000000 to 0xC03FFFFF) (thus mapping it in the
	# higher half). The kernel is identity mapped because enabling paging does
	# not change the next instruction, which continues to be physical. The CPU
	# would instead page fault if there was no identity mapping.
	# movl $(0xFD150000 | 0x003), boot_page_graphics - 0xC0000000 + 0 * 4
	# Map the page table to both virtual addresses 0x00000000 and 0xC0000000.
	movl $(boot_page_table1 - 0xC0000000 + 0x03), boot_page_directory - 0xC0000000 + 0


	movl $(boot_page_table1 - 0xC0000000 + 0x03), boot_page_directory - 0xC0000000 + 768 * 4

    # movl $(boot_page_table2 - 0xC0000000 + 0x003), boot_page_directory - 0xC0000000 + 1022
	movl $(boot_page_table2 - 0xC0000000 + 0x03), boot_page_directory - 0xC0000000 + 769 * 4

	# starts at FF800000


    # map a bunch of empty pages:
    mov NUMPAGES, %cl
    # set esi to the first page table
    movl $(boot_page_table3 - 0xC0000000 + 0x03) , %esi

    # boot_page_directory
    movl $(boot_page_directory - 0xC0000000 + 770 * 4), %edi

    # counter
    movl $770, %eax
l1:
    movl %esi, (%edi)

    inc %ebx
    addl $4, %edi
    addl $4096, %esi
    # movl $(boot_page_directory - 0xC0000000 + 771 * 4), %edi
    # movl $((boot_page_table3 + $4096) - 0xC0000000 + 0x003), %esi
    dec %cl
    jnz l1

    
3:
    # movl $(boot_page_table3 - 0xC0000000 + 0x003), boot_page_directory - 0xC0000000 + 770 * 4
    # movl $((boot_page_table3 + 4096) - 0xC0000000 + 0x003), boot_page_directory - 0xC0000000 + 771 * 4



	call _video_paging_init
    # movl $(0xFD150000 | 0x003), boot_page_graphics - 0xC0000000 + 0 * 4
	movl $(boot_page_graphics - 0xC0000000+ 0x03), boot_page_directory - 0xC0000000 + 1022 * 4
    movl $(boot_page_directory - 0xC0000000 + 0x03), boot_page_directory - 0xC0000000 + 1023 * 4

	# Set cr3 to the address of the boot_page_directory.
	movl $(boot_page_directory - 0xC0000000), %ecx
	
	movl %ecx, %cr3

	# Enable paging and the write-protect bit.
	movl %cr0, %ecx
	orl $0x80010000, %ecx
	movl %ecx, %cr0

	# Jump to higher half with an absolute jump. 
	lea 4f, %ecx
	jmp *%ecx

.section .text

4:
	# At this point, paging is fully set up and enabled.

	# Unmap the identity mapping as it is now unnecessary. 
	movl $0, boot_page_directory + 0

    # map the last page table to itself

	# Reload crc3 to force a TLB flush so the changes to take effect.
	movl %cr3, %ecx
	movl %ecx, %cr3

	# Set up the stack.
	mov $stack_top, %esp


	# Enter the high-level kernel.
	lea boot_data, %esi
	pushl %esi
	
	call kernel_main

	# Infinite loop if the system has nothing more to do.
	cli
1:	hlt
	jmp 1b


# IDT AND GDT

.global clearInterrupts
clearInterrupts:
    cli

.global setInterrupts
setInterrupts:
    sti

.section .rodata


.global gdt_flush     # Allows the C code to link to this
.extern gp            # Says that '_gp' is in another file
gdt_flush:
    lgdt (gp)        # Load the GDT with our '_gp' which is a special pointer
    jmp $0x08, $flush2   # 0x08 is the offset to our code segment: Far jump!
flush2:
    mov $0x10, %ax      # 0x10 is the offset in the GDT to our data segment
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss
    ret               # Returns back to the C code!
		

.global test_func
test_func:
	mov $0xC0DE, %eax
label:
	jmp label

# IDT
.global idt_load
.extern idtp
idt_load:
    lidt (idtp)
    ret

.global flush_tss
flush_tss:
	mov $0x2B, %ax
	ltr %ax
	ret

.global jump_usermode
.extern jump_usermode


jump_usermode:

	movl 4(%esp), %edi
	movl 8(%esp), %ecx
	movl %ecx, %esp
	mov $0x23, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs # ss is handled by iret

	# stack frame
	mov %esp, %eax
	push $0x23
	push %eax
	pushf
	
	push $0x1b
	push %edi

	iret
