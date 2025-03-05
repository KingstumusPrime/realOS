#include <kernel/syscalls/sysfile.h>
#include <stdio.h>
#include <string.h>
#include <kernel/multitasking/task_state.h>

// used for unique number
int fs_uid = 0;




int fd_num = 0;
superblock* superblocks[SB_COUNT] = {NULL};

int sb_count = 0;

file_system_type* last_fs = NULL;

int register_fs(file_system_type * fs){
    if(last_fs){
        last_fs->next = fs;
    }
    last_fs = &fs;
}

//gets the length till it hits a /
// example / returns 1, C:/ returns 3
// myName/ returns 7
int get_path_from_dir(char* path){
    for(int i = 0; i < 100; i++){
        if(path[i] == '/'){
            return i + 1;
        }
        if(path[i] == '\0'){
            return -1;
        }
    }
    return -1;
}



// reads from a path and may create a new one based on flags
// returns 0 success
// return -1 drive not found
// return -2 path not found
// return 1 path found (file created) 
dentry* check_path(char* target, int flags){
    if(target == NULL){
        return NULL;
    }
    bool match;
    // the base node of the drive
    dentry* root;
    superblock* block;
    int offset = 0;    
    // check drive mounts
    for(int i = 0; i < SB_COUNT; i++){
        match = false;
        if(superblocks[i] != NULL){
            block = superblocks[i];
            offset = 0; // offset from path
            match = true;
            while(get_path_from_dir((target + offset)) > 0){
                if(get_path_from_dir(target + offset) != get_path_from_dir((block->root->name + offset))){
                    match = false;
                    break;
                }


                for(int j = 0; j < get_path_from_dir(target + offset); j++){
                    if(block->root->name[offset + j] != target[offset + j]){
                        match = false;
                        break;
                    }
                }

                offset += get_path_from_dir((block->root->name + offset));
            }
            if(get_path_from_dir(target + offset) != get_path_from_dir((block->root->name + offset))){
                match = false;
            }
        }
        if(match){
            root = block->root;
            break;
        }
    }
    
    if(!match){

        return NULL;
    }

    // now we have the dentry check its children
   char* name = target + offset; // filler for the array

    offset = get_path_from_dir(name);

    // function keeps going till a return value
    bool at_file = false; // checks if at file 
    while (true)
    {
        
        if(offset == -1){
            // we are at the end of the file (check for the name)
            at_file = true;
            offset = strlen(name);
        }
        // get the size of the name
        // first check dentry
        if(root->children->next != NULL){
            list_item* i = root->children->next;
            bool match = false;
            while (i)
            {

                match = true;

                for(int j = 0; j < offset - 1; j++){
                    if(((dentry*)(i->data))->name[j] != name[j]){
                        match = false;
                        break;
                    }
                }

                if(match){
           
                    if (at_file && ((dentry*)i->data)->stale){
                        dentry* d = i->data;
                        i->data = root->node->ops->lookup(root->node, i->data, 0);
                        ((dentry*)i->data)->children = d->children; // direct swap with the stale entry
                        
                        d->children = NULL;
                        destroy_dentry(d);
                        root = i->data;
                        return root;
                    }
                    root = i->data;
                    name += offset;
                    offset = get_path_from_dir(name);
            
                    if(at_file){
                        return root;
                    }
                    break;
                }

                i = i->next;
            }
            if(match){
                continue;
            }
        }

        // create the base dentry
        dentry* en = malloc(sizeof(dentry));
        char* node_name = malloc(offset);
        // copy the name to name
        for(int i = 0; i < offset; i++){
            if(i == offset - 1 && !at_file){
                node_name[i] = '\0';
            }else{
                node_name[i] = name[i]; 
            }
        }

        en->parent = root;
        en->name = node_name;
        list_item* item = root->children;
        while (true)
        {
            if(item->next){
                item = item->next;
            }else{
                void* data = root->node->ops->lookup(root->node, en, 0);
                
                if(!data){
                    return NULL;
                }
                // we have to make a new entry
                list_item* new_item = malloc(sizeof(list_item));
                new_item->prev = item;
                item->next = new_item;
                new_item->data = data;
                ((dentry*)new_item->data)->children =  malloc(sizeof(list_item));
                ((dentry*)new_item->data)->children->data = ((dentry*)new_item->data);
                root = new_item->data;
                destroy_dentry(en);
                break;
            }
        }
    
        

        if(!root){
            return NULL;
        }
        if(at_file){
            return root;
        }
    
        name += offset;
        offset = get_path_from_dir(name);

    }

    return NULL;
}

// kernel version of open
int kopen(const char* name, int flags, int mode){
    // create a file
    file* res = malloc(sizeof(file));

    res->path = name;
    dentry* entry = check_path(name, flags);

    if(!entry || (entry->node->fops->open == NULL)){

        return -1;
    }

    entry->node->fops->open(entry->node, res);
    if(flags & O_EXCL){
        if(res->node){
            return -1;
        }
    }else{
        if(res->node == NULL){
            return -1;
        }
    }

    for(int i = 0; i < FILE_DESCRIPTOR_SIZE; i++){
        if(current->fdt[i] == NULL){
            current->fdt[i] = res; 
            return i;
        }
    }

    return -1;

}

void destroy_inode(inode *i){
    free(i->data);
    free(i);
}

void destroy_dentry(dentry* d){
    free(d->name);
    if(d->children){
        free(d->children);
    }
    free(d);
}

// kernel read syscall
int kread(int f_num, char *ptr, int len) {
  file* f = current->fdt[f_num];


  if(f == NULL|| (f->ops->read == NULL)){
    return -1;
  }  

  return f->ops->read(f, ptr, len, &f->f_pos);
}


// kernel read syscall
int kclose(int f_num) {
    file* f = current->fdt[f_num];
    dentry* entry = check_path(f->path, 0);
  
    entry->stale = true; // set the entry to stale

    free(f->path);
    free(f->node);
    destroy_inode(f->node);
    if(f->private_data){
        free(f->private_data);
    }

    current->fdt[f_num] = NULL;
    free(f);

}


// kernel read syscall
int kwrite(int f_num, char *ptr, int len) {
  file* f = current->fdt[f_num];

  if(!f || (f->ops->write == NULL)){
    return -1;
  }  

  return f->ops->write(f, ptr, len, &f->f_pos);
}

// kernel ioctl
int kioctl(int f_num, int func, va_list args) {

  void* arg = va_arg(args, void*); 

  file* f = current->fdt[f_num];
  if(!f || (f->ops->ioctl == NULL)){
    return -1;
  }  

  return f->ops->ioctl(f, func, arg);
}

// kernel lseek. sets a file position
int klseek(int fd, int ptr, int dir) {
    file* f = current->fdt[fd];
    if(!f){
        return -1;
    }
    int dest = 0;

    if(dir == SEEK_END){
        dest = f->node->size + 1 + ptr;
    }else if(dir == SEEK_CUR){
        //printf("SEEK CUR! %X\n", f->f_pos);
        dest = f->f_pos + ptr;
    }else{
        dest = ptr;
    }
    f->f_pos = dest;

    return dest;
}