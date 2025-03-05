#ifndef _KERNEL_DEVICE_FS_H
#define _KERNEL_DEVICE_FS_H
 
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

dentry* device_fs_mount(file_system_type * fs_type, int flags, const char *dev_name,void * data);
int register_device(const char* name, file_operations* fops);

extern file_system_type device_fs;
int device_file_open(inode* node, file* target);
dentry* device_lookup(inode* root, dentry* target, unsigned int flags);
#endif