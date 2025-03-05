#if defined(__is_libk)
#include <kernel/paging.h>
#endif

#include <stdio.h>
#include <string.h>

unsigned int base_addr = 0;
unsigned int last_addr = 0;
const PAGE_SIZE = 0x400000;
typedef struct
{
    int size;
    bool used;
} __attribute__((packed)) malloc_data;


// only works for one page pretty bad just for the vfs
void* malloc(size_t size){
    if(base_addr == 0){
#if defined(__is_libk)
        base_addr = mallocPage();
#else
// make this dynamic
        base_addr = 0x8400000;
#endif
        last_addr = base_addr;
    }




    void* i = base_addr;
    while (i < (last_addr - 1))
    {
        //printf("\nI =%X\n", i);
        if(*((bool*)(i + sizeof(int))) == 0 && *((int*)i) >= size){

            *((bool*)(i + sizeof(int))) = 1;

            return i +  (sizeof(int) + 1);
        } 
        i += ((*(int*)i) + (sizeof(int) + 1));
    }


    i = last_addr;
    *((int*)i) = size;

    *((bool*)(i + sizeof(int)))= 1;

    last_addr += (size + sizeof(int) + 1);

    return i + (sizeof(int) + 1);
    
}

void free(void* location){
    if(location == 0){
        return;
    }
    *((char*)location - 1) = 0;
}