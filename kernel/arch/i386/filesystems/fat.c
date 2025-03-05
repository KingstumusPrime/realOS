#include <kernel/fat.h>
#include <kernel/ata.h>
#include <stdio.h>
#include <string.h>

char digits[30] = "0123456789ABCDEF";

enum FAT_TYPES fatType = FAT_NONE;
int totalSectors;
int fatSize;
int rootDirSectors;
int firstDataSector;
int firstFatSector;
int dataSectors;
int totalClusters;
int firstRootDirSector;
int rootCluster32;
int sector_size;
int files_per_dir;
int sectors_per_cluster;
unsigned char* fatTableCache[80] = {NULL}; // cache 2 tables

typedef struct
{
    unsigned long cluster;
    unsigned long dir_cluster;
}  fat_inode_data;




void printHex(unsigned int num){
    bool printing = false;
    for(int i = 0; i < 8; i++){
        if(!printing){
            if( (num & ((0xF0000000) >> (i * 4))) > 0){
                printing = true;
            }
        }
        if(printing){
            printf("%c", digits[ ((num & ((0xF0000000) >> (i * 4))) >> (28 - (i * 4)))]);
        }
    }
    if(!printing){
        printf("0");
    }
}

void loadFatBS(unsigned char* bootsector, unsigned char* buffer){
    while (!drive_loaded);
    ata_read(0, 0, buffer);


    memcpy(bootsector, buffer, 36);
        printHex(buffer[0]);
}


unsigned short nextCluster(unsigned int activeCluster){
    switch (fatType){
        // bad function
        case FAT12:
            printf("ERROR!\n FAT 12 not supported");
            abort();
            // 2 sectors for if we border with 12 bits
            // unsigned char fatTable[1024];
            // unsigned int fatOffset = activeCluster + (activeCluster);
            // unsigned int fatSector = firstFatSector + (fatOffset/512);
            // unsigned int entOffset = fatOffset % 512;

            // ata_read(0, fatSector, fatTable);
            // ata_read(0, fatSector + 1, fatTable + 512);

            // unsigned short tableValue = *(unsigned short*)&fatTable[entOffset];
            

            // return tableValue & 0xFFF;
            break;
        case FAT16:
            unsigned int fatOffset = activeCluster << 1;
            unsigned int fatSector = firstFatSector + (fatOffset/512);
            unsigned char* fatTable = NULL;
            if(fatTableCache[fatSector]){

                fatTable = fatTableCache[fatSector];
            }else{
                printf("ALLOC! %X\n", fatSector);
                fatTable = malloc(sector_size);
                ata_read(0, fatSector , fatTable);
                fatTableCache[fatSector] = fatTable;
            }



            unsigned int off = fatOffset%512;
            //printf("SECTOR! %X\n", fatSector);


            unsigned short tableValue = *(unsigned short*)&fatTable[off];

            //printf("TB V! %X\n", tableValue);
            return tableValue;
            break;
            
        default:

            break;
    }
}


unsigned short allocateCluster(unsigned int activeCluster){
    switch (fatType){
        case FAT12:
            printf("ERROR!\n FAT 12 not supported");
            abort();
            // unsigned char fatTable[1024];
            // unsigned int fatOffset = activeCluster + (activeCluster);
            // unsigned int fatSector = firstFatSector + (fatOffset/512);
            // unsigned int entOffset = fatOffset % 512;

            // ata_read(0, fatSector, fatTable);
            // ata_read(0, fatSector + 1, fatTable + 512);

            // unsigned short tableValue = *(unsigned short*)&fatTable[entOffset];
            
            
            // if(tableValue <= 0xFF8){
            //     printf("ERROR!");
            //     return;
            // }
            // printf("ACTIVE CLUSTER %x\t%x\n", activeCluster, tableValue);
            // printf("FAT TABLE: \n");
            // unsigned char test[8] = {0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF,};
            // short v = 0;
            // for(char i = 0; i <= (0x40); i++){
            //     if(i & 1){
            //         v = (fatTable[(3 * i)/2] & 0xF0) << 4;
            //         v += (fatTable[(3 * i)/2 + 1] & 0xFF);
            //     }else{
            //         v = ((fatTable[(3 * i)/2 + 1] & 0xF));
            //         v += ((fatTable[(3 * i)/2]) << 4);
            //     }
            //     printf("%X\t%X\n", (i),  v);
            // }

            return 0;
            break;
        case FAT16:
            unsigned char* fat_table = malloc(sector_size);
            unsigned int fat_offset = activeCluster << 1;
            unsigned int fat_sector = firstFatSector + (fat_offset/sector_size);
            unsigned int off = fat_offset%sector_size;

            ata_read(0, fat_sector , fat_table);

            unsigned short table_value = *(unsigned short*)&fat_table[off];
            
            if(table_value < 0xFFF8){
                printf("ERROR NOT END THIS MAKES NO SENSE!\n");
                abort();
            }

            // first check this sector
            short found = 0;
            for(int i = 0; i < sector_size/2; i += 2){

                if(fat_table[i] == 0 && fat_table[i + 1] == 0 ){
                    found = i/2;
                    break;
                }
            }

            if(found > 0){
                fat_table[off] = found & 0xFF;
                fat_table[off + 1] = (found >> 8);
                fat_table[found * 2] = 0xFF;
                fat_table[found * 2 + 1] = 0xFF;
                ata_write(0, fat_sector, &fat_table);

                free(fatTableCache[fat_sector]);
                fatTableCache[fat_sector] = fat_table;
                return found;
            }

            // check the whole thing
            unsigned char* fat_table_ref = malloc(sector_size);
            int sector_num;
            for(sector_num = firstFatSector; sector_num < fatSize; sector_num++){
                // already checked this sector
                if(sector_num == fat_sector){
                    continue;
                }

                ata_read(0, sector_num, fat_table_ref);

                for(int i = 0; i < sector_size/2; i += 2){
                    if(fat_table_ref[i] == 0 && fat_table_ref[i + 1] == 0 ){
                        found = i/2;
                        break;
                    }
                }
                if(found > 0){
                    break;
                }
            }    
        
            if(found > 0){
                fat_table[off] = found & 0xFF;
                fat_table[off + 1] = (found >> 8);
                fat_table_ref[found * 2] = 0xFF;
                fat_table_ref[found * 2 + 1] = 0xFF;

                ata_write(0, fat_sector, &fat_table);
                ata_write(0, sector_num, &fat_table_ref);

                // cache fat table
                free(fatTableCache[fat_sector]);
                fatTableCache[fat_sector] = fat_table;

                //cache fat table ref
                if(sector_num < 80){
                    free(fatTableCache[fat_sector]);
                    fatTableCache[sector_num] = fat_table_ref;
                }


                // free(fat_table);
                // free(fat_table_ref);
                return found;
            }

            
            free(fat_table);
            free(fat_table_ref);

            return 0;
            break;
        default:
            break;
    }
}

void fatINIT(){
    FATBR bootsector;
    unsigned char buffer[512];
    loadFatBS(&bootsector.bootJump, buffer);
    //printHex(0xCAB);
    printf("\nBoot jump: ");
    printHex(buffer[0]);
    printHex(buffer[1]);
    printHex(buffer[2]);
    printf("\n");


    // useful values
    if (bootsector.sectorCount == 0){
        totalSectors = bootsector.largeSectorCount;
    }else{
        totalSectors = bootsector.sectorCount;
    }

    sector_size = bootsector.bytesPerSector;
    sectors_per_cluster = bootsector.sectorsPerCluster;
    rootDirSectors = ((bootsector.rootEntries * 32) + (bootsector.bytesPerSector - 1)) / bootsector.bytesPerSector;
    firstFatSector = bootsector.reservedSectors;

    // fill extended boot records
    FATEBR ebr;
    FAT32EBR ebr32;

    memcpy(&ebr, (buffer + 36), 476);
    memmove((unsigned char*)&ebr32, (buffer + 36), 476);

    if (bootsector.sectorsPerFAT == 0){
        fatSize = ebr32.sectorsPerFAT;
    }else{
        fatSize = bootsector.sectorsPerFAT;
    }

    dataSectors = totalSectors - (bootsector.reservedSectors + (bootsector.FATCount * fatSize) + rootDirSectors);

    totalClusters = dataSectors / bootsector.sectorsPerCluster; 

    // TODO get make 512 dynamic
    files_per_dir = (512 * bootsector.sectorsPerCluster)/32;

    if(totalSectors == 0){
        fatType = EXFAT;
    }else if(totalClusters < 4085){
        fatType = FAT12;
    }else if(totalClusters < 65525){
        fatType = FAT16;
    }else{
        fatType = FAT32;
    }


    // drive specific code
    switch(fatType){
        case EXFAT:
            printf("EXFAT DRIVE NOT SUPPORTED (yet)\n");
            break;
        case FAT12:
            printf("FAT12 DRIVE NOT SUPPORTED (yet)\n");
            // fill extended boot record
            memcpy(&ebr, (buffer + 36), 476);
            
            break;
        case FAT16:
            printf("FAT16 DRIVE NOT SUPPORTED (yet)\n");
            // fill extended boot record
            memcpy(&ebr, (buffer + 36), 476);
            break;
        case FAT32:
            printf("FAT32 DRIVE NOT SUPPORTED (yet)\n");
            printHex(buffer[38]);

            break;
    }



    firstDataSector = bootsector.reservedSectors + (bootsector.FATCount * fatSize ) + rootDirSectors;
    dataSectors = totalSectors - (firstFatSector + (bootsector.FATCount * fatSize) + rootDirSectors);

    if(fatType == FAT32){
        rootCluster32 = ebr32.rootClusterNum;
        printf("\nFAT32 drive detected printing some info\n");
        printf("total clusters: ");
        printHex(totalClusters);
        printf("\nroot cluster: ");
        printHex(rootCluster32);
    }else if(fatType != EXFAT){
    

        firstRootDirSector = firstDataSector - rootDirSectors;


        return;
        unsigned char* rootBuffer = malloc(512);
        unsigned char* clusterBuffer = malloc(bootsector.sectorsPerCluster * 512);
        ata_read(0, firstRootDirSector, rootBuffer);

        if(buffer[0] == 0){
            printf("NO FILES FOUND");
            return;
        }else{
            printf("FILE FOUND\n");

            // check for all files in the buffer
            for(int i = 0; i < 32; i++){
                // load file info into data structure
                dirEntry d;
                memcpy((unsigned char*)&d, rootBuffer + (32 * i), 32);
                
                // quit the loop if no more files
                if( d.name[i] == 0){
                    break;
                }


                // print file
                // print title
                printf("file name: ");
                for(int i = 0; i < 11; i++){
                    // 0x20 is the blank space
                    if(d.name[i] != 0x20){
                        printf("%c", d.name[i]);
                    }

                    if(i == 7){
                        printf(".");
                    }
                }

                printf("\n");

                // follow cluster chain
                
                // given the cluster send the sector
                int cluster = d.clusterNumLow;
                while (cluster < 0xFF7 && cluster != 0)
                {
                    int sectorOfFile = ((cluster - 2) * bootsector.sectorsPerCluster) + firstDataSector;
                    // load the cluster
                    for(char sectOfClust = 0; sectOfClust < bootsector.sectorsPerCluster; sectOfClust++){
                        ata_read(0, sectorOfFile + sectOfClust, clusterBuffer + (sectOfClust * 512));
                    }
                    
                    // print the whole cluster
                    unsigned int sectorsPassed = (cluster -  d.clusterNumLow) * (bootsector.sectorsPerCluster * 512);
                    for (size_t j = 0; j + sectorsPassed < d.sizeInBytes && j < bootsector.sectorsPerCluster * 512; j++)
                    {
                        printf("%c", clusterBuffer[j]);
                    }


                    printf("Cluster is: ");
                    printHex(cluster);
                    printf("\n");
                    cluster = nextCluster(cluster);

                    
                
                }
            
            }
            
            
        }
    }
}



// returns the cluster or dir_cluster if it does not exist
// TODO long files
file_info fat_find_dir(unsigned long dir_cluster, char* target_name){

    // read the directory
    // maby make this global so we art reallocating
    unsigned char* clusterBuffer= malloc(sectors_per_cluster * 512);
    unsigned long cluster = dir_cluster;
    int sect = dir_cluster;
    if(cluster >= firstDataSector){
        cluster = ((cluster - firstDataSector)/sectors_per_cluster) + 2;
        sect = ((cluster - 2) * sectors_per_cluster) + firstDataSector;
    }

    

    file_info f = {
        0, FS_ERR, 0
    };

    // while there are more clusters
    while (cluster < 0xFF7 && cluster != 0)
    {
        // fill cluster buffer 
        
        for(int i = 0; i<  sectors_per_cluster; i++){
            ata_read(0, sect + i, clusterBuffer + (512* i));
        }


        // search the cluster TODO add a check if its a directory
        // basic flag
        bool name_match = false;
        // for each file
        for(int i = 0; i < files_per_dir; i++){
            // create a directory entry. This is kind of slow but readable
            dirEntry d;
            memcpy((unsigned char*)&d, clusterBuffer + (32 * i), 32);
            // assume the name matches
            name_match = true;

            // needed to bypass formatting such as blank 0x20 and the . not in the name
            char char_in_name = 0;
            for(int j = 0; j < 11; j++){
                // no more files (return file not found)
                if(j == 0 && d.name[j] == 0){
                    return f;
                }
                // skip the .
                if(j == 8){char_in_name++;}
                // skip spaces
                if(d.name[j] == 0x20){continue;}

                // if a character does not align set flag and go to next file 
                if(d.name[j] != target_name[char_in_name]){
                    name_match = false;
                    //break;
                }
                char_in_name++;
            }
            // return the cluster equation
            if(name_match){
                if((d.attributes & 0x10) > 0){
                    f.type = FS_DIR;
                }else{
                    f.type = FS_FILE;
                }
                f.cluster = (((d.clusterNumLow + (d.clusterNumHigh << 16))- 2) * sectors_per_cluster) + firstDataSector;
                f.size = d.sizeInBytes;
                return f;
            }

        }
        cluster = nextCluster(cluster);
        sect = ((cluster - 2) * sectors_per_cluster) + firstDataSector;
    }

    // failed
    return f;
}

// VFS
file_system_type fat_fs = {
    "FAT",
    0,
    NULL,
    &fat_mount
};

// dentry ops

unsigned long fat_size(){
    return totalClusters;
}

unsigned long fat_space(){
    return -1;
}

unsigned int fat_root_cluster(){
    switch (fatType)
    {
    case FAT32:
        return rootCluster32;
        break;
    case FAT16:
    case FAT12:
        return firstRootDirSector;
    default:
        printf("UNSUPPORTED DRIVE");
        break;
    }
}

// super block operations
static super_operations fat_super_ops =
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

static inode_operations fat_node_ops = {
    NULL, // create dentry
    NULL, // rename
    &fat_lookup, // find
};

static file_operations fat_file_ops = {
   &fat_file_open,
   &fat_read,
   &fat_write,
   NULL
};

// no dentry ops yet
dentry_operations fat_dentry_ops;




// VFS functions
dentry* fat_mount(file_system_type * fs_type, int flags, const char *dev_name,void * data){
    fatINIT();
    dentry* root = malloc(sizeof(dentry));

    // say dev_name was C:/ or /dev/sda1
    root->name = dev_name;   
    // root points to himself
    root->parent = root;

    // create the inode for the root
    inode* root_node = malloc(sizeof(inode));
    root_node->o_uid = fs_uid++;

    root_node->type = FS_DIR;
    root_node->ops = &fat_node_ops;
    root->stale = false;

    // create superblock
    superblock* root_sb = malloc(sizeof(superblock));
    root_sb->s_list = superblocks;
    superblocks[sb_count++] = root_sb;
    root_sb->blocksize = sector_size;
    root_sb->fs_type = fs_type;
    root_sb->ops = &fat_super_ops;
    root_sb->uid = fs_uid++;
    // root_sb->inodes = &root_node->children;
    root_sb->root = root;
    // dentry ops
    root_sb->d_ops = &fat_dentry_ops;

    root_node->sb = root_sb;
    root_node->node_number = fs_uid++;
    root_node->size = fat_size();
    root_node->bytes_used = fat_space();
    root_node->fops = &fat_file_ops;
    root_node->data = malloc(sizeof(fat_inode_data));

    ((fat_inode_data*)root_node->data)->cluster = fat_root_cluster();
    ((fat_inode_data*)root_node->data)->dir_cluster = 0;
    
    root->node = root_node;
    root->d_ops = &fat_dentry_ops;
    // points to nothing
    list_item* i = malloc(sizeof(list_item));
    i->data = root;
    i->next = NULL;
    i->prev = NULL;
    root->children = i;

    return root;  
}

// TODO add flags
dentry* fat_lookup(inode* root, dentry* target, unsigned int flags){
    file_info f_info = fat_find_dir(((fat_inode_data*)root->data)->cluster, target->name);


    if(f_info.type == FS_ERR){
        return NULL;
    }

    inode* node = malloc(sizeof(inode));
    node->o_uid = 0;
    node->flags = 0;
    node->type = f_info.type;
    node->ops = &fat_node_ops;
    node->sb = root->sb;
    node->node_number = fs_uid++;
    node->size = f_info.size;// get size
    // skip bytes used
    node->fops = &fat_file_ops;
    node->data = malloc(sizeof(fat_inode_data));
    ((fat_inode_data*)node->data)->cluster = f_info.cluster;
    ((fat_inode_data*)node->data)->dir_cluster = ((fat_inode_data*)(root->data))->cluster;
    
    dentry* d = malloc(sizeof(dentry));
    d->name = target->name;
    // no parent
    // node
    d->node = node;
    d->d_ops = &fat_dentry_ops;
    d->stale = false;
    // no children
    d->children = NULL;
    return d;
}

int fat_file_open(inode* node, file* target){
    // cannot do path only data and file_operations
    target->node = node;
    target->ops = &fat_file_ops;
    target->flags = 0; // empty for now
    target->private_data = &(((fat_inode_data*)node->data)->cluster);

    return 0;
}

size_t fat_read(file* file, char* buffer, size_t size,  size_t* offset){
    // read root
    unsigned char* clusterBuffer = malloc(sectors_per_cluster * 512);
    unsigned long cluster = ((fat_inode_data*)file->node->data)->cluster;
    cluster = ((cluster - firstDataSector)/sectors_per_cluster) + 2; // convert the sector to a cluster

    unsigned int res = 0;

    // while there are more clusters
    size_t off = *offset; // placeholder 
    if(off >= file->node->size){
        //printf("FILE NODE LESS\nOFF %X SIZE %X\n\n\n", off, file->node->size);
        return 0;
    }
    while(off >= sectors_per_cluster * sector_size){
        cluster = nextCluster(cluster);
        off -= sectors_per_cluster * sector_size;
    }
    // the position in the buffer
    int buff_pos = 0;
    int oref = 0;
    while (cluster < 0xFFF8  && cluster != 0)
    {
        int sect = ((cluster - 2) * sectors_per_cluster) + firstDataSector;

        // fill cluster buffer 
        // modify this to only read the max amount
        // example 512, 1024 
        for(int i = 0; i<  sectors_per_cluster; i++){
            ata_read(0, sect + i, clusterBuffer + (512* i));
        }       
        oref = off;

        // optimize this with a bulk memcpy
        for(int i = off; i < sectors_per_cluster  * 512; i++, buff_pos++){
            if(buff_pos >= size || off + buff_pos >= file->node->size){
                //printf("EARLY RETURN! %X %X\n", i, sectors_per_cluster  * 512);
                return res;
            }
            buffer[buff_pos] = clusterBuffer[i];
            res++;
        }

        // int amount = sectors_per_cluster  * 512;
        // if(buff_pos >= size){
        //     //printf("NO ACTAUL WAY %X %X\n\n\n\n\n", buff_pos, size);
        //     return res;
        // }else if(off + amount >= file->node->size){
        //     amount = (off + amount) - file->node->size;
        // }

        // memcpy(buffer + buff_pos, clusterBuffer, amount);
        // buff_pos += amount;
        // res += amount;
        off = 0;
        // next cluster
        cluster = nextCluster(cluster);

    }
    
    if(res == 0){
        //printf("WELP RES WAS JUST ZERO\n\n");
        //printf("size %X cluster %x\n",  sectors_per_cluster  * 512, cluster);
    }
    return res;
}

int fat_set_size(file* file, int new_size){

    char* dir = malloc(sector_size * sectors_per_cluster);

    unsigned long search_cluster = ((fat_inode_data*)(file->node->data))->cluster;
    search_cluster = ((search_cluster - firstDataSector)/sectors_per_cluster) + 2; // convert the sector to a cluster

    unsigned long cluster = ((fat_inode_data*)file->node->data)->dir_cluster;
    unsigned int sect = cluster;
    if(cluster > firstDataSector){
        cluster = ((cluster - firstDataSector)/sectors_per_cluster) + 2; // convert the sector to a cluster
        sect = ((cluster - 2) * sectors_per_cluster) + firstDataSector;
    }


    while (cluster < 0xFF7 && cluster != 0)
    {
        


        // fill cluster buffer 
        // modify this to only read the max amount
        // example 512, 1024 
        for(int i = 0; i<  sectors_per_cluster; i++){
            ata_read(0, sect + i, dir + (sector_size * i));
        }       

        for(int i = 0; i < sector_size * sectors_per_cluster; i += 32 ){

            dirEntry* e = dir + i;
            // for(int j = 0; j < 32; j++){
            //     printf("%c", dir[j + i]);
            // }
            // printf("\n");
            // printf("%X\t%X\n", e->clusterNumLow + (e->clusterNumHigh << 8), search_cluster);

            if(e->clusterNumLow + (e->clusterNumHigh << 8) == search_cluster){
                e->sizeInBytes = new_size;
                file->node->size = new_size;

                for(int i = 0; i<  sectors_per_cluster; i++){
                    ata_write(0, sect + i, dir + (512* i));
                }       

                free(dir);
                return 0;
            }
        }

        cluster = nextCluster(cluster);
        sect = ((cluster - 2) * sectors_per_cluster) + firstDataSector;
    }

    free(dir);
    return -1;
}

size_t fat_write(file* file, char* buffer, size_t size, size_t* offset){
    // write the buffer
    // void ata_write(int slave_bit, uint32_t LBA, unsigned char buffer[512]){
    unsigned long cluster = ((fat_inode_data*)file->node->data)->cluster;
    cluster = ((cluster - firstDataSector)/sectors_per_cluster) + 2; // convert the sector to a cluster

    int off = *offset;
    int f_pos = 0;

    bool extending = false;


    while(off >= sectors_per_cluster * sector_size){

        off -= sectors_per_cluster * sector_size;
        f_pos += sectors_per_cluster * sector_size;

        
        if((f_pos)/1024 > file->node->size/1024){
            extending = true;
            file->node->size = (f_pos + 512);
            cluster = allocateCluster(cluster);
        }else{
            cluster = nextCluster(cluster);
        }
    }

    int sect = ((cluster - 2) * sectors_per_cluster) + firstDataSector; // the sector

    if(off >= 512){
        sect++;
        off -= 512;
        f_pos += 512;
    }
    f_pos = off;
    char sect_read = 0;
    int bytes_left = size;

    int size_in_bytes = file->node->size;

    while (bytes_left > 0)
    {
        if(off > 0){


            if((f_pos + (512 - off))/1024 > file->node->size/1024){
                extending = true;
                file->node->size = (f_pos + 512);
                cluster = allocateCluster(cluster);
            }

            // create a sector buffer
            char* buff = malloc(sizeof(char) * sector_size);

            // read it
            ata_read(0, sect, buff);

            // overwrite only the areas we want
            memcpy(buff + (sizeof(char) * off), buffer, (512 - off));

            // write the modfyied buffer back
            ata_write(0, sect, &buff);

            // free buffer
            free(buff);
 
            // we know offset is 0
            bytes_left -= (512 - off);
            f_pos += (512 - off);
            off = 0;
        }else if(bytes_left < 512){

            if((f_pos + bytes_left)/1024 > file->node->size/1024){
                extending = true;
                file->node->size = (f_pos + 512);
                cluster = allocateCluster(cluster);
            }

            // create a sector buffer
            char* buff = malloc(sizeof(char) * sector_size);

            // read it
            ata_read(0, sect, buff);

            memcpy(buff, buffer, bytes_left);
        
            ata_write(0, sect, &buff);

            free(buff);
            f_pos += bytes_left;
            bytes_left = 0;
        }else{
            
            if((f_pos + 512)/1024 > file->node->size/1024){
                extending = true;
                file->node->size = (f_pos + 512);
                cluster = allocateCluster(cluster);
            }
            ata_write(0, sect, &buffer);
            bytes_left -= 512;
            f_pos += 512;
        }

        // next sector
        sect_read++;
        if((sect_read%sectors_per_cluster) == 0 && !extending){
            cluster = nextCluster(cluster);
            sect = ((cluster - 2) * sectors_per_cluster) + firstDataSector; // the sector
        }else{
            sect++;
        }
    }
    // edge case for super small file extending
    if(size > file->node->size && !extending){
        extending = true;
        file->node->size = size;
    }
    if(extending){
        fat_set_size(file, f_pos);
    }

    return size;
}