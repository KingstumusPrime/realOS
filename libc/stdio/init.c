// includes all of the base STDIO functions
// open close, ect..

#include <stdio.h>
#include <string.h>


#if defined(__is_libk)
#include <kernel/syscalls/sysfile.h>
int (*sys_write)(int, char*, int) = &kwrite;
int (*sys_lseek)(int, char*, int) = &klseek;
int (*sys_read)(int, char*, int) = &kread;
int (*sys_open)(int, char*, int) = &kopen;
int (*sys_close)(int) = &kclose;
#else
#include <syscalls.h>
int (*sys_write)(int, char*, int) = &write;
int (*sys_lseek)(int, char*, int) = &lseek;
int (*sys_read)(int, char*, int) = &read;
int (*sys_open)(int, char*, int) = &open;
int (*sys_close)(int) = &close;
#endif
 
#ifndef FILE_DESCRIPTOR_SIZE
#define FILE_DESCRIPTOR_SIZE 25
#endif 


FILE* stdin = NULL;
FILE* stdout = NULL;
FILE* stderr = NULL;

FILE *fopen(const char* restrict filename, const char* restrict mode) {


    FILE* f = malloc(sizeof(FILE));

    

    // TODO add flags using mode

    int f_num = (*sys_open)(filename, 0, 0);

    
    //f_num = (*sys_open)(filename, 0, 0);
    if(f_num == -1){
        
        return NULL;
    }

    //file* fd = file_descriptors[f_num];
    f->char_width = 1; // one byte wide
    f->buffer_state = LINE;


    // set the buffer
    char* buff = (char*)malloc(sizeof(char) * BUFSIZ);
    f->buffer = buff;


    
    if(mode[0] == 'r'){

        f->io_mode = INPUT;
        (*sys_read)(f_num, f->buffer, BUFSIZ);
        f->buf_size = BUFSIZ;
        //(*sys_lseek)(f_num, BUFSIZ, SEEK_CUR);
    }
    else if(mode[0] == 'w'){

        f->io_mode = OUTPUT;

        (*sys_read)(f_num, f->buffer, BUFSIZ);

        f->buf_size = BUFSIZ;
 
        
    }
    else if(mode[0] == 'a'){

        f->io_mode = UPDATE;
        
        (*sys_read)(f_num, f->buffer, BUFSIZ);
        f->buf_size = BUFSIZ;
        //(*sys_lseek)(f_num, BUFSIZ, SEEK_CUR);
    }

    
    if(mode[1] != '\0'){
        if(mode[1] == "+" || (mode[2] == '+' && mode[1] == 'b')){
            f->io_mode = UPDATE;
        }

    }

    int p = (*sys_lseek)(f_num, 0, SEEK_CUR);

    f->size = (*sys_lseek)(f_num, 0, SEEK_END) - 1;//fd->node->size;

    
    (*sys_lseek)(f_num, p, SEEK_SET);



    f->text_mode = true;
    f->eof = false;
    f->pos = 0;
    f->error = 0;
    f->lock = false;
    f->descriptor = f_num;
    f->dynamic_buf = true;
    f->last_char = EOF;
    fseek(f, 0, SEEK_SET);
    return f;


}

FILE *freopen(const char* restrict filename, const char* restrict mode, FILE* restrict stream){
    printf("BUGGY freopen");
    return fopen(filename, mode);
}


int fclose(FILE *stream ){
    if(stream == NULL){
        return EOF;
    }
    fflush(stream);

    (*sys_close)(stream->descriptor);
    if(stream->dynamic_buf){
        free(stream->buffer);
    }
    free(stream);
    return 0;
}


int ungetc(int c, FILE* stream){
    if(stream->last_char != EOF){
        return -1;
    }
    stream->last_char = c;
    return 0;
}

// optimized the sys_lseek
int fgetcNoPos(FILE* stream, int pos){
    int res;

    if(stream->last_char != EOF){
        res = stream->last_char;
        stream->last_char = EOF;
        return res;
    }

    if(stream->eof){return EOF;}

    if(stream->pos + pos  >= stream->size){
        printf("EOF!");
        stream->eof = true;
        return EOF;
    }
    if(stream->pos >= stream->buf_size){
        //printf("BUFFER GRAB\n");
        int amount = (*sys_read)(stream->descriptor, stream->buffer, stream->buf_size);
        if(amount == 0){
            //printf("THIS AGAIN!\n");
            stream->eof = true;
            return EOF;
        }
        stream->pos = 0;
        (*sys_lseek)(stream->descriptor, amount, SEEK_CUR);
        //file_descriptors[stream->descriptor]->f_pos += amount;
    }

    res = stream->buffer[stream->pos];
    stream->pos++;
    return res;
}


int fgetc(FILE* stream){
    int res;

    if(stream->last_char != EOF){
        res = stream->last_char;
        stream->last_char = EOF;
        return res;
    }

    if(stream->eof){return EOF;}

    if(stream->pos + ((*sys_lseek)(stream->descriptor, 0, SEEK_CUR))  >= stream->size){
        printf("EOF!");
        stream->eof = true;
        return EOF;
    }
    if(stream->pos >= stream->buf_size){

        (*sys_lseek)(stream->descriptor, stream->buf_size, SEEK_CUR);
        int amount = (*sys_read)(stream->descriptor, stream->buffer,stream->buf_size );

        if(amount == 0){
            //printf("THIS AGAIN!\n");
            stream->eof = true;
            return EOF;
        }
        stream->pos = 0;
        
        //file_descriptors[stream->descriptor]->f_pos += amount;
    }

    res = stream->buffer[stream->pos];
    stream->pos++;
    return res;
}


int fseek(FILE *stream, long int offset, int whence){
    stream->eof = false;
    //int p = stream->pos + (file_descriptors[stream->descriptor]->f_pos);
    unsigned int p = ftell(stream);
    unsigned int dest = 0;


    if(whence == SEEK_END){
        dest = stream->size + 1 + offset;

    }else if(whence == SEEK_CUR){
        dest = p + offset;


    }else{
        //printf("SEEK SET OFF = %i\n", offset);
        dest = offset;
    }


    if(dest > p){
        // more than 512

        if((dest/stream->buf_size) > (p/stream->buf_size)){

            //file_descriptors[stream->descriptor]->f_pos = (((dest)/stream->buf_size)) * stream->buf_size;
           
            (*sys_lseek)(stream->descriptor, (((dest)/stream->buf_size)) * stream->buf_size, SEEK_SET);
           
            int amount = (*sys_read)(stream->descriptor, stream->buffer, stream->buf_size);


            if(amount <= 0){
                
                stream->eof = true;
                return EOF;
            }

            //file_descriptors[stream->descriptor]->f_pos += stream->buf_size;
            //(*sys_lseek)(stream->descriptor, stream->buf_size, SEEK_CUR);
        }
        stream->pos = (dest)%stream->buf_size;

    }else{
        if((dest/stream->buf_size) < (p/stream->buf_size)){
            //printf("%X : %X\n\n\n\n\n\n\n\n", (dest)/stream->buf_size, (p/stream->buf_size) );
            // file_descriptors[stream->descriptor]->f_pos = (((dest)/stream->buf_size)) * stream->buf_size;
            (*sys_lseek)(stream->descriptor,  (((dest)/stream->buf_size)) * stream->buf_size, SEEK_SET);
            //printf("THIS IS %X\n", file_descriptors[stream->descriptor]->f_pos);
            int amount = (*sys_read)(stream->descriptor, stream->buffer, stream->buf_size);
            if(amount == 0){
                stream->eof = true;
                return EOF;
            }
            //file_descriptors[stream->descriptor]->f_pos += stream->buf_size;
            //(*sys_lseek)(stream->descriptor, stream->buf_size, SEEK_CUR);
        }
        stream->pos = (dest)%stream->buf_size;

        //printf("MYSTR %x %x %x\n", stream->pos, stream->pos, fgetc(stream));
    }

}

long int ftell(FILE *stream){
    return (unsigned int)(*sys_lseek)(stream->descriptor, 0, SEEK_CUR) + (unsigned int)stream->pos;
}

// TODO SET ERROR
int fputc(int c, FILE* restrict stream){
    if(stream->eof){return EOF;}
    if(stream->pos < stream->buf_size){
        stream->buffer[stream->pos++] = c;
    }else{
        return EOF;
    }

    int fpos = (*sys_lseek)(stream->descriptor, 0, SEEK_CUR);

    switch (stream->buffer_state)
    {
    case NONE:
        fflush(stream);
        break;
    case LINE:
        if(c == '\n'){fflush(stream);}
        break;
    case FULL:
        if(stream->pos - 1 + (fpos * (stream->size > 512))  >= stream->size){
            fflush(stream);
        }
        break;
    default:
        break;
    }
    return c;
}


int fflush(FILE *stream){
    // loop over all files and find one
    if(stream == NULL){
#if defined(__is__libk) 
        for(int i = 0; i < FILE_DESCRIPTOR_SIZE; i++){
            if(file_descriptors[i] != NULL && 
            (stream->io_mode == OUTPUT ||  stream->io_mode == UPDATE)){
                fflush(file_descriptors[i]);
            }
        }
#else
    return -1;
#endif
    }else{
        if(stream->io_mode == UPDATE || stream->io_mode == OUTPUT ){

            (*sys_write)(stream->descriptor, stream->buffer, stream->pos);
            int p = (*sys_lseek)(stream->descriptor, stream->pos, SEEK_CUR);
            //file_descriptors[stream->descriptor]->f_pos += stream->pos;
            // update file size
            //stream->size = file_descriptors[stream->descriptor]->node->size;
            stream->size = ((*sys_lseek)(stream->descriptor, 0, SEEK_END) - 1);

            (*sys_lseek)(stream->descriptor, p, SEEK_SET);
            stream->pos = 0;
            return 0;
        }else{
            return -1;
        }
    }
}