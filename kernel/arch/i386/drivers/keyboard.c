#include <kernel/keyboard.h>
#include <kernel/drivers/kterminal.h>
#include <termios.h>
#include <kernel/syscalls/sysfile.h>
#include <kernel/device/tty.h>
#include <stdio.h>
/* KBDUS means US Keyboard Layout. This is a scancode table
*  used to layout a standard US keyboard. I have left some
*  comments in to give you an idea of what key is what, even
*  though I set it's array index to 0. You can change that to
*  whatever you want using a macro, if you wish! */
unsigned char kbdus[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};	


unsigned char keyboard_buffer[256];
uint8_t keyboard_buffer_index = 0;

void keyboard_handler(struct regs *r){
    // check the echo command
    tty_device* dev = ((tty_device*)file_descriptors[1]->node->data);
    unsigned int flag = ((struct termios*)dev->term)->c_lflag;

    char key = inb(0x60);
    if((key & 0x80) == 0 && (flag & ECHO)){
      if(key == 72){
        //printf("UP");
        (*terminal_scroll_up)();
      }else if(key == 80 && (flag & ECHO)){
        //printf("DOWN");
        (*terminal_scroll_down)();
      }else{
        if((flag & ECHO) > 0){
          printf("%c", kbdus[key]);
        }

      }
    }

    if(keyboard_buffer_index < 255){
      keyboard_buffer[keyboard_buffer_index++] = key;
    }
        

    if((flag & ICANON) == 0 || kbdus[(key & (~0x80))] == '\n'){
      
      keyboard_buffer[keyboard_buffer_index+1] = '\0';
      memcpy(dev->buf + dev->bpos++, keyboard_buffer, keyboard_buffer_index + 1);
      memset(keyboard_buffer, 0, keyboard_buffer_index + 1);
      keyboard_buffer_index = 0;
    }

}