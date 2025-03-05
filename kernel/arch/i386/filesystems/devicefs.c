#include <kernel/syscalls/sysfile.h>
#include <kernel/fs/devicefs.h>


file_system_type device_fs = {
    "devicefs",
    0,
    NULL,
    &device_fs_mount
};



// no dentry ops yet
dentry_operations devices_dentry_ops;

// super block operations
static super_operations device_super_ops =
{
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL  
};

static inode_operations device_node_ops = {
    NULL, // create dentry
    NULL, // rename
    &device_lookup, // find
};

static file_operations device_file_ops = {
   NULL,
   NULL,
   NULL,
   NULL
};

dentry* root = NULL;

int register_device(const char* name, file_operations* fops){
    if(!root){
        return NULL;
    }
    dentry* device = malloc(sizeof(dentry));
    device->parent = root;
    inode* node = malloc(sizeof(inode));
    device->name = name;
    device->node = node;
    device->d_ops = &devices_dentry_ops;

    list_item* i = malloc(sizeof(list_item));
    i->data = device;
    i->next = NULL;
    i->prev = root;
    device->children = i;
    // add this to the root
    // i = root->chi
    i = root->children;
    while (i->next != NULL)
    {
        i = i->next;
    }
    i->next = device->children;

    node->o_uid = 0;
    node->flags = 0;
    node->type = FS_FILE;
    node->ops = &device_node_ops;
    node->sb = root->node->sb;
    node->node_number = fs_uid++;
    node->size = 0;
    node->bytes_used = 0;
    node->fops = fops;
    node->data = 0;

}

dentry* device_fs_mount(file_system_type * fs_type, int flags, const char *dev_name,void * data){
    root = malloc(sizeof(dentry));

    root->name = dev_name;
    // root points to himself
    root->parent = root;
    // no inode because this is just a place holder
    // the devices themselves will have inodes
    inode* node = malloc(sizeof(inode));
    root->node = node;

    root->d_ops = &devices_dentry_ops;

    superblock* root_sb = malloc(sizeof(superblock));
    root_sb->s_list = superblocks;
    superblocks[sb_count++] = root_sb;
    root_sb->blocksize = 0;
    root_sb->fs_type = fs_type;
    root_sb->ops = &device_super_ops;
    root_sb->uid = fs_uid++;
    root_sb->root = root;
    root_sb->d_ops = &devices_dentry_ops;

    list_item* i = malloc(sizeof(list_item));
    i->data = root;
    i->next = NULL;
    i->prev = NULL;
    root->children = i;


    node->o_uid = 0;
    node->flags = 0;
    node->type = FS_DIR;
    node->ops = &device_node_ops;
    node->sb = root_sb;
    node->node_number = fs_uid++;
    node->size = 0;
    node->bytes_used = 0;
    node->fops = &device_file_ops;
    node->data = 0;

    return root;
}


dentry* device_lookup(inode* root, dentry* target, unsigned int flags){
    return NULL;
}


int device_file_open(inode* node, file* target){
    printf("OPENED!!!!\n\n\n");
    // cannot do path only data and file_operations
    target->node = node;
    target->ops = &device_file_ops;
    target->flags = 0; // empty for now
    target->private_data = 0;

    return 0;
}