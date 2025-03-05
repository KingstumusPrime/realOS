#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

 
#include <kernel/tty.h>
#include <kernel/bootdata.h>
 
#include "vga.h"
 
#define VGA_WIDTH  80
#define VGA_HEIGHT  25
//static uint16_t* const VGA_MEMORY = (uint16_t*) 0xB8000;
// new paged
static uint16_t* const VGA_MEMORY = (uint16_t*) FB_ADDR;
 
size_t terminal_row;
size_t terminal_column;
static uint8_t terminal_color;
static uint16_t* terminal_buffer;
// entries 0-2534are for the top of the terminal, entries 255-509 are for the bottom
static unsigned char terminal[510][VGA_WIDTH * 2];
// a set of pointers to find the next place to put 
static unsigned short terminal_top_next = 0;
static unsigned short terminal_bottom_next = 250;

 
void tty_terminal_initialize(void) {
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = VGA_MEMORY;
	// loop and clear terminal
	for(int i = 0; i< 510; i++){
		for(int j = 0; j < VGA_WIDTH * 2; j++){
			terminal[i][j] = vga_entry(' ', terminal_color);
		}
	}
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}
 
// sets color
void tty_terminal_setcolor(uint8_t color) {
	terminal_color = color;
}
 
// put char at
void tty_terminal_putentryat(unsigned char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}
 
void tty_terminal_scroll_down() {

	// copy the line we lost into the terminal array so we can get it later
	memcpy(terminal + (VGA_WIDTH * terminal_top_next), terminal_buffer, VGA_WIDTH * 2);
	terminal_top_next++;
	// copy each line to the line before
	for(int i = 1; i < VGA_HEIGHT; i++){
		for(int j = 0; j < VGA_WIDTH; j++){
			terminal_buffer[((i-1) * VGA_WIDTH) + j] = terminal_buffer[(i * VGA_WIDTH) + j];
		}
	}
	
	//tty_terminal_delete_last_line();
	// copy the bottom line
	memcpy(terminal_buffer + VGA_WIDTH * (VGA_HEIGHT - 1), terminal + (VGA_WIDTH * (terminal_bottom_next - 1)), VGA_WIDTH * 2);
	if(terminal_bottom_next > 0){
		terminal_bottom_next--;
	}
	terminal_column = 0;
}


void tty_terminal_scroll_up() {

	// copy the line we lost into the terminal array so we can get it later
	memcpy(terminal + (VGA_WIDTH * terminal_bottom_next), terminal_buffer + VGA_WIDTH * (VGA_HEIGHT - 1), VGA_WIDTH * 2);
	terminal_bottom_next++;

	// copy the bottom line
	memcpy(terminal_buffer, terminal + (VGA_WIDTH * (terminal_top_next - 1)), VGA_WIDTH * 2);
	if(terminal_top_next > 0){
		terminal_top_next--;
	}


	// copy each line to the line before
	for(int i = VGA_HEIGHT - 1; i > 0; i--){
		memcpy(terminal_buffer + (VGA_WIDTH * i), terminal_buffer + (VGA_WIDTH * (i - 1)), VGA_WIDTH * 2);
	}

	terminal_column = 0;
}
 
 
void tty_terminal_delete_last_line() {
	for(int i = 0; i < VGA_WIDTH; i++){
		terminal_buffer[VGA_WIDTH * (VGA_HEIGHT - 1) + i] = vga_entry_color(' ', terminal_color);
	}
}
 
void tty_terminal_putchar(char c) {
	int line;
	unsigned char uc = c;
	if(uc == '\n'){
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT)
		{
			tty_terminal_scroll_down();
			terminal_row--;
		}
		return;
	}
	tty_terminal_putentryat(uc, terminal_color, terminal_column, terminal_row);
	if (++terminal_column == VGA_WIDTH) {
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT)
		{
			tty_terminal_scroll_down();
			terminal_row--;
		}
	}
}
 
void tty_terminal_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++)
		tty_terminal_putchar(data[i]);
}
 
void tty_terminal_writestring(const char* data) {
	tty_terminal_write(data, strlen(data));
}