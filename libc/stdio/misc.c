#include <stdio.h>
#include <string.h>
 
#if defined(__is_libk)
#include <kernel/syscalls/sysfile.h>
#endif


// includes some random functions
int setvbuf(FILE* restrict stream, char* restrict buf, int mode, size_t size){

    if(!stream) return -1;
    if(stream->descriptor < 0) return -1;
    printf("%X\n", stream->buffer);

    if(stream->dynamic_buf){
        printf("FREE!\n");
        free(stream->buffer);
    }
    stream->buffer = buf;
    printf("%X\n", stream->buffer);
    stream->buffer_state = mode;


    if(stream->io_mode != OUTPUT && (mode == LINE || mode == FULL)){

        //file_descriptors[stream->descriptor]->f_pos -= stream->buf_size;
        klseek(stream->descriptor, (stream->buf_size * -1), SEEK_CUR);
        kread(stream->descriptor, stream->buffer, size);

        //file_descriptors[stream->descriptor]->f_pos += size;
        klseek(stream->descriptor, size, SEEK_CUR);
    }
    
    stream->buf_size = size;

    
    return 0;
}


void setbuf(FILE* restrict stream, char* restrict buf){
    
    if(buf){
        setvbuf(stream, buf, FULL, BUFSIZ);
    }else{
        setvbuf(stream, buf, NONE, BUFSIZ);   
    }
}