#ifndef _KERNEL_SYSFILE_H
#define _KERNEL_SYSFILE_H

#define SB_COUNT 20

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

// max of 20 files
#define FILE_DESCRIPTOR_SIZE 20
// add a define



typedef struct list_head list_head;
typedef struct list_item list_item;
typedef struct dentry dentry;
typedef struct dentry_operations dentry_operations;
typedef struct file_system_type file_system_type;
typedef struct super_operations super_operations;
typedef struct superblock superblock;
typedef struct inode inode;
typedef struct file file;

#include <kernel/multitasking/task_state.h>

extern int fs_uid;
extern int sb_count; // index for superblocks
extern superblock* superblocks[SB_COUNT];

struct list_head
{
    list_head *prev;
    list_head *next;
};


struct list_item
{
    list_item *prev;
    list_item *next;
    void* data;
};

// placeholder
typedef struct
{
    unsigned long bytes_left;
    unsigned long total_size;
    
} statfs;


struct super_operations
{
    void (*read_inode) (inode *);
    int (*notify_change) (inode *);
    void (*write_inode) (inode *);
    void (*put_inode) (inode* );
    void (*put_super) (superblock*);
    void (*write_super) (superblock*);
    void (*statfs) (superblock*, statfs*, int); // need to add statfs
    int (*remount_fs) (superblock*, int*, char*);
};


struct superblock
{
    int* s_list; // points to a list of super blocks
    unsigned long blocksize; // size of a block
    file_system_type *fs_type;
    const super_operations *ops;
    int uid;
    //list_item *inodes;  // linked list of other nodes
    dentry *root;
    dentry_operations *d_ops;
};


typedef struct
{
    int (*create) (inode*, dentry*, int);
    int (*rename) (inode*, dentry*, inode*, dentry*);
    dentry* (*lookup) (inode*, dentry*, unsigned int);
} inode_operations;


typedef struct
{
    int (*open) (inode*, file*);
    size_t (*read) (file *, char *, size_t,  size_t*);
    size_t (*write) (file *, char *, size_t,  size_t*);
    size_t (*ioctl) (file *, int, void*);
 } file_operations;

typedef enum{
    FS_FILE,
    FS_DIR,
    FS_ERR
}FILE_TYPE;

struct inode
{
    /* data */
    int o_uid; // owner uid
    unsigned int flags;
    FILE_TYPE type;
    const inode_operations *ops;
    superblock *sb;
    unsigned long node_number;
    unsigned long size; // size in bytes
    unsigned long bytes_used;
    const file_operations* fops;
    void* data; // pointer to the data.
    // for a FAT filesystem this could be a cluster number of the first
};

// none for now
struct dentry_operations{

};


struct dentry{
    const char* name;
    dentry *parent;
    inode* node;
    const dentry_operations* d_ops;
    list_item *children; // pointer to the list head for the child
    bool stale;
};





struct file_system_type
{
    const char* name; // file system name
    int fs_flags; // useless
    file_system_type *next; // because linked lists rock
    dentry *(*mount)(file_system_type *, int, const char *, void*);

};


typedef enum {
    F_READ,
    F_WRITE,
    F_DUEL,
} fmode_t;

struct file{
    char* path;
    inode* node;
    const file_operations* ops;
    unsigned int flags;
    uint64_t f_pos;
    void* private_data;
};

int register_fs(file_system_type *);

// int read(int fd, char* buf, size_t count);
// int open(const char* pathname, int flags, mode_t mode);


// sycalls stuff
#define O_RDONLY 1 // read only
#define O_WRONLY 2 // write only
#define O_RDWR 4 // open for read and write
#define O_CREAT 8 // create if it does not exist
#define O_TRUNC 16 // truncate to zero if it exists
#define O_APPEND 32 // append data
#define O_EXCL 64 // error if the file exists
#define file_descriptors (current->fdt)
void destroy_inode(struct inode * i);
void destroy_dentry(dentry* d);
dentry* check_path(char* target, int flags);
int klseek(int fd, int ptr, int dir);
int kwrite(int f_num, char *ptr, int len);
int kread(int f_num, char *ptr, int len);
int kopen(const char* name, int flags, int mode);
int kclose(int f_num);


#endif