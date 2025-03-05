#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <kernel/tty.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/ata.h>
#include <kernel/fat.h>
#include <kernel/paging.h>
#include <kernel/syscalls/sysfile.h>
#include <kernel/fs/devicefs.h>
#include <kernel/device/tty.h>
#include "kernel/inline.h"

#include <kernel/bootdata.h>
#include <kernel/drivers/pci.h>
#include <kernel/drivers/fb_terminal.h>
#include <kernel/multitasking/task_state.h>
#include <kernel/multitasking/multitasking.h>
#include <kernel/elf.h>


#define WIDTH 1024
#define HEIGHT 768
#define SIZE WIDTH * HEIGHT

#define SERIAL_PORT 0x3F8

void init_serial() {
    outb(SERIAL_PORT + 1, 0x00);    // Disable all interrupts
    outb(SERIAL_PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    outb(SERIAL_PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
    outb(SERIAL_PORT + 1, 0x00);    //                  (hi byte)
    outb(SERIAL_PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
    outb(SERIAL_PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    outb(SERIAL_PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}

int is_transmit_empty() {
	return inb(SERIAL_PORT + 5) & 0x20;

}

void write_serial(char a) {
    while (is_transmit_empty() == 0);
 	outb(SERIAL_PORT, a);
}

void printf_serial(const char* str) {
    while (*str) {
        write_serial(*str++);
    }
}

void my_func(int* p){
	p += 10;
}

unsigned char* f = "0123456789ABCDEF";
void printf_serial_hex(unsigned int num, int len){
	write_serial('0');
	unsigned int mask = (0xF << ((len - 1) * 4));
	for(int i = 0; i <= len; i++){
	if(i == 0){
	// 		write_serial("0");
	}else{
		int x = (num & mask) >> ((len - i) * 4);
		write_serial(f[x]);
		mask = mask >> 4;
	}
	}
}
extern void mov_eax(int);
extern void mov_ebx(int);
extern unsigned get_esp();
void bar(){
	// printf("BAR");
	//set_eax(0xDEAD);
	__asm__ __volatile__ (
        "int $0x80" 
    );

	while (1)
	{
		/* code */
	}
	
}


extern void jump_usermode(unsigned int, unsigned int);



void a(){
	printf("A\n");
	volatile bool running = true;
	switch_task(pqueue[0]);
    while (running)
    {    
    }
	printf("DONE A!\n");
}

void b(){
	printf("B\n");
	volatile bool running = true;
	//init_task(pqueue[0], &b);
	switch_task(pqueue[1]);
    while (running){    
    }
}

void kernel_main(boot_data_t* addr) {

	

	terminal_init(addr);
	// sets super important flags used by string functions
	cpu_data_init(addr);
	printf_serial("paging\n");

	// turn on paging
	paging_init();

	printf_serial("paging\n");
	gdt_install();
	printf_serial("GDT\n");
	idt_install();
	printf_serial("IDT\n");
	check_all_buses();
	// PRDT
	map_page(0x80000000, 0xFF400000,0x07);

	ata_init();

	// init multitasking
	multitasking_init();

	// create a dummy proccess for the kernel
	current_task = malloc(sizeof(task_state));
	current_task->fdt = malloc(sizeof(file) * FILE_DESCRIPTOR_SIZE);
	current_task->esp = get_esp();
	current->pc = NULL;

    __asm__ volatile ("sti");

	printf_serial("INERRUOTS\n");
	init_serial();
	printf_serial("Buffer is: ");
	printf_serial_hex(addr->fb_width, 8);
	printf_serial("\n");

	

	

	char x = 'x';
	unsigned char* s = "my char x =";
	void* nums[5];

	// fb_term_init(addr);
	char* str = "abc";
	// buffer_write_string("HELLO\nWORLD");
	// printf_serial("INERRUOTS\n");

	printf("ADDR = %X\n", addr);
	printf("ADDR = %X\n", &x);

	printf("Hello, kernel World! %s %c\n", s, x);

	pci_device_t* dev = pci_dev_root;
	while (dev != NULL)
	{
		printf("[%X:%X:%X] : %X/%X/%X\n", dev->bus, dev->device, dev->function,
		dev->baseclass, dev->subclass, dev->lf);
		dev = dev->next;
	}



	
	// register the file system
	register_fs(&fat_fs);
	// mount file sytem
	fat_fs.mount(&fat_fs, 0, "/", NULL);

	// mount device fs:
	register_fs(&device_fs);
	device_fs.mount(&device_fs, 0, "/dev/", NULL);
	register_device("keyboard", &tty_ops);
	register_device("tty", &tty_ops);
	// stdout = fopen("/dev/tty", "w");
	// stdin = fopen("/dev/keyboard", "r");
	// stderr = fopen("/dev/tty", "w");
	
	// FILE* wad = fopen("/WAD.WAD", "r");
	// printf("Wad: %x\n", wad);
	// printf("HEAD: ");
	// while (wad->eof == false){
	// 	printf("%c", fgetc(wad));
	// }
	// printf("\n");
	// while (1)
	// {
	// 	/* code */
	// }


	
	// unsigned char* buf = malloc(10);
	// snprintf(buf, 10, "hello %.3d", 38);
	// for(int i = 0; i < 10; i++){
	// 	printf("%c", buf[i]);
	// }

	
	// while (2)
	// {
	// 	/* code */
	// }
	
	// for(int i = 0; i < 100; i++){
	// 	if(i%10 ==0){
	// 		printf("READ %i\n", read_pit_count());
	// 	}
	// }
	// while (1)
	// {
	// 	/* code */
	// }
	


	FILE* f_g  = fopen("/TOP.ELF", "r");


	elf_header* elf_data = malloc(sizeof(elf_header));

	for(char i = 0; i < 51; i++){
		((char*)elf_data)[i] = fgetc(f_g);
	}

	FB_TERM_COLOR = 0x0000FF00;
	printf("===ELF HEADER DATA===\n");
	printf("MAGIC = ");
	for(int i = 0; i < 4; i++){
		printf("%c", elf_data->magic[i]);
	}
	printf("\n");

	printf("%u BIT ELF\n", elf_data->bit_count == 1 ? 32 : 64);

	if(elf_data->instruction_set == 0x03){
		printf("x86 INSTRUCTION SET\n");
	}else{
		printf("UNKOWN INSTRUCTION SET: %X\n", elf_data->instruction_set);
	}

	printf("TYPE %u\n", elf_data->type);

	printf("%s ENDIAN\n", elf_data->endian == 1 ? "LITTLE" : "BIG");

	printf("HEADER OFFSET %X\n", elf_data->header_off);

	printf("SECTION OFFSET %X\n", elf_data->sect_off);

	printf("ENTRY COUNT %X\n", elf_data->entry_count);

	printf("ENTRY OFFSET %X\n", elf_data->entry_off);

	printf("STRING TABLE: %X\n", elf_data->sect_off + (elf_data->string_table ));

	printf("DATA SIZE %u\n", sizeof(elf_header));

	fseek(f_g, elf_data->header_off, SEEK_SET);


	map_page(0x38000000, 0x8000000,0x07);
	map_page(0x38400000, 0x8400000,0x07);
	map_page(0x38800000, 0x8800000,0x07);
	map_page(0x38800000, 0x8800000,0x07);
	map_page(0x38C00000, 0x8C00000,0x07);
	map_page(0x39000000, 0x9000000,0x07);
	map_page(0x39400000, 0x9400000,0x07);


	elf_program_head* elf_head = malloc(sizeof(elf_program_head));;

	// add_task(&b);
	// add_task(&a);
	// switch_task(pqueue[0]);
	//add_task(&a);
	printf("TASKS\n");
	//switch_task(pqueue[0]);

	fseek(f_g, elf_data->header_off, SEEK_SET);
	printf("AHH = %X\n", elf_data->header_off);
	// add_task(&a);



	for(int i = 0; i < elf_data->entry_count; i++){
		

		for(char i = 0; i < sizeof(elf_program_head); i++){
			((char*)elf_head)[i] = fgetc(f_g);
		}

		printf("===  HEADER DATA  ===\n");

		printf("TYPE %u\n", elf_head->type);
		printf("OFFSET %u\n", elf_head->offset);

		printf("VADDR 0x%X\n", elf_head->vaddr);

		printf("PADDR 0x%X\n", elf_head->paddr);
		printf("SIZE %u\n", elf_head->segment_size);
		printf("MEM SIZE %u\n", elf_head->mem_size);

		fseek(f_g, elf_head->offset, SEEK_SET);

		unsigned char* p = elf_head->vaddr;
		//unsigned char* v = &bar;
		printf("P %X!\n", p);
		for(int i = 0; i <   elf_head->mem_size; i++){
			if(i < elf_head->segment_size){
				*(p + i) = fgetc(f_g);
			}else{
				*(p + i) = 0;
			}
		}
		// for(int i = 0; i < elf_head->mem_size - elf_head->segment_size; i++){
		// 	*(p + i = 0)
		// }
	printf("DONE!\n\n");
		fseek(f_g, elf_data->header_off + (sizeof(elf_program_head) * (i + 1)), SEEK_SET);
		printf("DONE!\n\n");
	}

	int elf_end = elf_head->vaddr + (elf_head->mem_size); // where the stack will go
	free(elf_head);

	printf("= %X\n", elf_data->entry_off);
	int stack_size = 4096;
	set_kernel_stack(get_esp());



	printf("STACK = %X\n", elf_end + stack_size);
	jump_usermode((unsigned int)elf_data->entry_off, elf_end + stack_size);
	//void *ptr = (void *)elf_data->entry_off;  // a random memory address
	//goto *ptr; 
       
	while(1)
	{
		/* code */
	}
	
}