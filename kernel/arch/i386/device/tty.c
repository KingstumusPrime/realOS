#include <kernel/device/tty.h>
#include <termios.h>
#include <string.h>



file_operations tty_ops = {
   &tty_open,
   &tty_read,
   &tty_write,
   &tty_ioctl,
};




int tty_open(inode* node, file* target){
    // cannot do path only data and file_operations
    target->node = node;
    target->ops = &tty_ops;
    target->flags = 0; // empty for now
    tty_device* dev = malloc(sizeof(tty_device));
    struct termios* t =  malloc(sizeof(struct termios));
    memset(t, 0, sizeof(struct termios));

    t->c_lflag = (ECHO | ICANON);

    
    dev->term = t;
    dev->buf = malloc(256);
    dev->bufsz = 256;
    dev->bpos = 0;
    target->node->data = dev;

    return 0;
}

size_t tty_read(file* f, char* buffer, size_t size,  size_t* off){

	tty_device* dev = (tty_device*)f->node->data;
    if(size >= dev->bufsz){
        memcpy(buffer,dev->buf,dev->bufsz);
    }else{
        ///printf("HI!\n");

        memcpy(buffer,dev->buf, size);
        //printf("BUF %s ", dev->buf);
        memcpy(dev->buf, dev->buf + size, ( dev->bufsz - size));
        if(dev->bpos >= size){
            dev->bpos -= size;
        }


    }
    
    return size;
}
size_t tty_write(file* f, char* buffer, size_t size,  size_t* off){
	//printf("WRITE");
	for(int i = 0; i < size; i++){
		printf("%c", buffer[i]);
	}
	return size;
}

size_t tty_ioctl(file* file, int func, void* d){
    switch (func)
    {
    case TCGETS:

        memcpy(d, ((tty_device*)file->node->data)->term, sizeof(struct termios));
        break;
    case TCSETS:

        memcpy(((tty_device*)file->node->data)->term, d, sizeof(struct termios));
        break;
    default:
        break;
    }
}