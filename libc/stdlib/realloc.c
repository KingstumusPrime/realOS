#include <string.h>
#include <stdlib.h>
#include <stdio.h>
// does its job (KINDA)
// DO NOT USE IT!!!!!!!
// ONLY FOR DOOM
void* realloc(void* addr, size_t size){

    if(size == 0){
        return NULL;
    }
    if(addr == 0){
        return malloc(size);
    }
    int* org_sz = (int*)((char*)addr - 5);// orginal size

    if(*org_sz > size){

        *org_sz = size;
        return addr;
    }else{

        free(addr);
        return malloc(size);
    }
}