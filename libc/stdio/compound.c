// includes compound functions that are built of others such as fread/fwrite
#include <stdio.h>
#include <string.h>



size_t fread(void* restrict ptr, size_t size, size_t nmemb, FILE* restrict stream){

    
    if(nmemb == 0 || size == 0){
        return EOF;
    }

    unsigned int bytes = size * nmemb;
    unsigned int pos = ftell(stream);
    unsigned int start = pos;
    unsigned int off = 0;
    unsigned char* buf = (unsigned char*)ptr;
    
    while (bytes != 0)
    {
        if(stream->buf_size - stream->pos > bytes ){
            memcpy(buf + off, stream->buffer + stream->pos, bytes);
            off += bytes;
            pos += bytes;

            if(fseek(stream, bytes,SEEK_CUR) == EOF
            || pos > stream->size){
                bytes = 0;
                fseek(stream, 0, SEEK_END);
                return (stream->size - start)/size;
            }
            bytes = 0;
        }else{

            memcpy(buf + off, stream->buffer + stream->pos, stream->buf_size - stream->pos);

            bytes -= (stream->buf_size - stream->pos);
            off += (stream->buf_size - stream->pos);
            pos += (stream->buf_size - stream->pos);
            
            if(fseek(stream, (stream->buf_size - stream->pos),SEEK_CUR) == EOF
            || pos >= stream->size){
                fseek(stream, 0, SEEK_END);
                return (stream->size - start)/size;
            }
        }

    }
    

    return nmemb;
}


size_t fwrite(const void* restrict ptr, size_t size, size_t nmemb, FILE* restrict stream){
    if(nmemb == 0 || size == 0){
        return EOF;
    }

    char* data = ptr;
    for(size_t i = 0; i < nmemb; i++){
        for(size_t j = 0; j < size; j++){
         if(fputc(data[(size * i) + j], stream) == EOF){
            return i;
         }

        }
    }

    return nmemb;
}