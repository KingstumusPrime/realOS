#ifndef _KERNEL_ELF_H
#define _KERNEL_ELF_H
 
#include <stddef.h>
#include <stdint.h>

typedef struct {
    char magic[4];
    unsigned char bit_count;
    unsigned char endian; // 1 = little, 2 = big
    unsigned char header_version;
    unsigned char ABI; // zero for system V
    unsigned char padding[8];
    unsigned short type; // 1 relocatable, 2 = executable, 3 = shared, 4 = core
    unsigned short instruction_set;
    unsigned int version;
    unsigned int entry_off;
    unsigned int header_off;
    unsigned int sect_off;
    unsigned int flags;
    unsigned short head_size;
    unsigned short entry_size;
    unsigned short entry_count;
    unsigned short section_entry_size;
    unsigned short section_count;
    unsigned short string_table;
} __attribute__((__packed__)) elf_header;


typedef struct {
    unsigned int type;
    unsigned int offset;
    unsigned int vaddr;
    unsigned int paddr;
    unsigned int segment_size;
    unsigned int mem_size;
    unsigned int flags;
    unsigned int alignment;
} __attribute__((__packed__)) elf_program_head;
#endif