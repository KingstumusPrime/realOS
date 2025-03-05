#include <kernel/paging.h>
#include <kernel/fat.h>
#include <stdbool.h>
#include <string.h>

#define TABLE_COUNT 10


// reserved pages (the size of the kernel in pages)
const int RESERVED_PAGES = 2;

// the page we start at
const int FIRST_PAGE = 770;
// the number of pages that can be allocated
unsigned int* curr_table = (unsigned int*)(FIRST_PAGE * 0x400000);
// set to the last free page
unsigned int empty_page_addr = 0xC3000000; // the addr were we store empty addresses

void *get_physaddr(void *virtualaddr) {
    unsigned long pdindex = (unsigned long)virtualaddr >> 22;
    unsigned long ptindex = (unsigned long)virtualaddr >> 12 & 0x03FF;

    unsigned long *pd = (unsigned long *)0xFFFFF000;
    // Here you need to check whether the PD entry is present.

    unsigned long *pt = ((unsigned long *)0xFFC00000) + (0x400 * pdindex);
    // Here you need to check whether the PT entry is present.

    return (void *)((pt[ptindex] & ~0xFFF) + ((unsigned long)virtualaddr & 0xFFF));
}

void set_page(void *physaddr, void *virtualaddr, unsigned int flags, bool clear) {

    // Make sure that both addresses are page-aligned.
    //printf("PHYS ADDR = %X", (unsigned long)physaddr);
    unsigned long pdindex = (unsigned long)virtualaddr >> 22;
    unsigned long ptindex = (unsigned long)virtualaddr >> 12 & 0x03FF;

    unsigned long *pd = (unsigned long *)0xFFFFF000;
    unsigned long *pt = (unsigned long *)0xFFC00000;


    // Here you need to check whether the PD entry is present.
    // When it is not present, you need to create a new empty PT and
    // adjust the PDE accordingly.
    


    if(pd[pdindex] & 1) {
        unsigned long *pt = ((unsigned long *)0xFFC00000) + (0x400 * pdindex);

        if(!pt){
            //printf("NO PD!");
        }
        for(int i = 0; i < 1024; i++){
            if(clear){
                pt[i] = 0;
            }else{
                pt[i] = ((unsigned long)physaddr + (i * 0x1000)) | flags;
            }

        }
    }else{

        unsigned long* new_pt = empty_page_addr;
        empty_page_addr += 0x4000;

        pd[pdindex] = ((unsigned long)(get_physaddr(new_pt))) | flags;
        
        new_pt[((unsigned long)virtualaddr) >> 12] = ((unsigned long)physaddr) | flags;

        unsigned long *pt = ((unsigned long *)0xFFC00000) + (0x400 * pdindex);

        for(int i = 0; i < 1024; i++){
            if(clear){
                pt[i] = 0;
            }else{
                pt[i] = ((unsigned long)physaddr + (i * 0x1000)) | flags;
            }

        }

    }
    // Now you need to flush the entry in the TLB
    // or you might not notice the change.
    for(int i = 0; i < 1024; i ++){
        asm volatile("invlpg (%0)" : : "r" (virtualaddr + (i * 0x1000)));
    }

}

void map_page(void *physaddr, void *virtualaddr, unsigned int flags) {
    set_page(physaddr, virtualaddr, flags, false);
}

void clear_page(void *physaddr, void *virtualaddr) {
    set_page(physaddr, virtualaddr, 0, true);
}

// allocates a page - 3 bytes
unsigned int* mallocPage(){
    // return the current table
    unsigned int* return_val = curr_table;

    // calculate the next one
    unsigned int* next_table = ((*curr_table) * 0x400000);
    if(next_table == curr_table){
        return -1;
    }else{
        curr_table = next_table;
    }

    // return the current table before we overwrite it
    return (return_val + 3);
}

// allocates a page - 3 bytes
freePage(unsigned int* page){
    unsigned short curr_table_num = (unsigned int)curr_table/0x400000;
    // subtract 3 so we are at the true start and overwrite the header
    *(page - 3) = curr_table_num;
    curr_table = (page - 3);

}

void paging_init(){
    // init the table
    unsigned short* x;
    for(int i = 0; i < 10; i++){
        map_page((void*)(0x800000 + (0x400000 * i)), (void*)(0xC0800000 + (0x400000 * i)), 7);
        //map_page(0x800000 + (0x400000 * i), 0xC0800000 + (0x400000 * i), 3);
        x = (unsigned short*)(0xC0800000 + (0x400000 * i));
        if(i != 9){
            *x = FIRST_PAGE + i + 1;
        }else{
            *x = FIRST_PAGE + i;
        }

    }
    empty_page_addr = mallocPage();
}