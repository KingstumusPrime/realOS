#ifndef _KERNEL_FAT_H
#define _KERNEL_FAT_H
 
#include <stddef.h>
#include <stdint.h>
#include <kernel/syscalls/sysfile.h>


enum FAT_TYPES{
    FAT_NONE, FAT12, FAT16, FAT32, EXFAT, FAT_UNKOWN
};

typedef struct FATBR {
    unsigned char bootJump[3];
    unsigned char OEM[8];
    unsigned short bytesPerSector;
    unsigned char sectorsPerCluster;
    unsigned short reservedSectors;
    unsigned char FATCount;
    unsigned short rootEntries;
    unsigned short sectorCount;
    unsigned char mediaDescriptorType;
    unsigned short sectorsPerFAT;
    unsigned short sectorsPerTrack;
    unsigned short sidesOnMedia;
    unsigned int hiddenSectors;
    unsigned int largeSectorCount;
} __attribute__((packed)) FATBR;


typedef struct FATEBR {
    unsigned char driveNumber;
    unsigned char flags;
    unsigned char signature;
    unsigned int serial;
    unsigned char label[11];
    unsigned char sysID[8];
    unsigned char bootCode[448];
    unsigned short BPA;
} __attribute__((packed)) FATEBR;

typedef struct FAT32EBR {
    unsigned int sectorsPerFAT;
    unsigned short flags;
    unsigned short version;
    unsigned int rootClusterNum;
    unsigned short FSInfoSector;
    unsigned short backupBootSector;
    unsigned char reserved[12];
    unsigned char driveNumber;
    unsigned char winFlags;
    unsigned char signature;
    unsigned int serial;
    unsigned char label[11];
    unsigned char sysID[8];
    unsigned char bootCode[420];
    unsigned short BPA;
} __attribute__((packed)) FAT32EBR;

typedef struct FSInfo {
    unsigned int leadSignature;
    unsigned char reserved[480];
    unsigned int signature;
    unsigned int lastKnownFreeCluster;
    unsigned int nextClustter;
    unsigned char reserved2[12];
    unsigned int trailSignature;
    
} __attribute__((packed)) FSInfo;

typedef struct dirEntry {
    unsigned char name[11];
    unsigned char attributes;
    unsigned char reserved;
    unsigned char creationTime;
    unsigned short fileCreationTime;
    unsigned short fileCreationDate;
    unsigned short accessedDate;
    unsigned short clusterNumHigh;
    unsigned short modTime;
    unsigned short modDate;
    unsigned short clusterNumLow;
    unsigned int sizeInBytes;
} __attribute__((packed)) dirEntry;


// returned by find operations
typedef struct
{
    unsigned long cluster;
    FILE_TYPE type;
    unsigned long size;
} file_info;


void loadFatBS(unsigned char* bootsector, unsigned char buffer[512]);
void fatINIT();
unsigned short nextCluster(unsigned int activeCluster);
void printHex(unsigned int num);
dentry* fat_mount(file_system_type * fs_type, int flags, const char *dev_name, void* data);
file_info fat_find_dir(unsigned long dir_cluster, char* target_name);
int fat_file_open(inode* node, file* target);
dentry* fat_lookup(inode* root, dentry* target, unsigned int flags);
size_t fat_read(file* file, char* buffer, size_t size,  size_t* offset);
size_t fat_write(file* file, char* buffer, size_t size, size_t* offset);
int fat_set_size(file* file, int new_size);
extern file_system_type fat_fs;

#endif