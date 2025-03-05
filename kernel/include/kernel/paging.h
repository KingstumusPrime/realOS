#ifndef _KERNEL_PAGING_H
#define _KERNEL_PAGING_H

// for now this is it
void paging_init();
unsigned int* mallocPage();
#endif