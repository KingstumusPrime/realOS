#include <kernel/inline.h>
#include <kernel/ata.h>
#include <kernel/drivers/pci.h>
#include <stdio.h>
#include <stdbool.h>
#define noop (void)0

bool drive_loaded = false;
volatile bool irq_received = false;

// char c[30] = "0123456789ABCDEF";

// void print16(uint16_t n){
//     printf("%c", c[((n >> 4) & 0xF)]);
//     printf("%c", c[(n & 0xF)]);
//     printf("%c", c[((n >> 12) & 0xF)]);
//     printf("%c", c[((n >> 8) & 0xF)]);
// }

pci_device_t* drive = NULL;
// map_page(0x80000000, 0xFF400000,0x07);
// only one entry with physcal address 0x80000000
// virtual address 0xFF400000
// 512 bytes
unsigned long long* prdt = 0xFF400000; 
unsigned int bar4;



void delay_400ns(){
    // inb(ATA_ALT_STATUS);
    // inb(ATA_ALT_STATUS);
    // inb(ATA_ALT_STATUS);
    // inb(ATA_ALT_STATUS);
}

const char ata_errors[8][30] = {
    "Address mark not found.",
    "Track zero not found.",
    "Aborted command.",
    "Media change request.",
    "ID not found",
    "Media changed",
    "Uncorrectable data error.",
    "Bad Block detected"
};

void showErrorTypes(){
    printf("ERROR\n");
}

void ATA_wait_BSY(){
    int status = inb(ATA_STATUS);
    while ( (status & 0x80) > 0)
    {
        status = inb(ATA_STATUS);
    }
    
}

void ATA_wait_DRQ(){
    int status = inb(ATA_STATUS);
    while ( (status & 0x8) == 0)
    {
        status = inb(ATA_STATUS);
    }
    
}

char tb[512];


void ata_dma_drive(bool write, int addr, int slavebit, unsigned char* buf){
    irq_received = false;
    // set direction
    outb(bar4, 0x0); // clear the transfer bit
    if(!write){
        outb(bar4, 0x8); // set the read bit 
    } // otherwise the bit is already zero

    // clear the irq and error bits
    outb(bar4 + 2, 4);

    // set LBA and sect count
    ATA_wait_BSY();

    outb(ATA_DRIVE, 0xE0 | (slavebit << 4) | ((addr >> 24) & 0x0F));
    outb(ATA_FEATURES, 0x00);
    int sector_count = 1; 
    outb(ATA_SECTOR_COUNT, (unsigned char)sector_count);
    outb(ATA_SECTOR_NUM, (unsigned char) addr);
    outb(ATA_LOW, (unsigned char) (addr>> 8));
    outb(ATA_HIGH, (unsigned char) (addr >> 16));

    //ATA_wait_DRQ();

    if(write){
        outb(ATA_COMMAND, 0xCA);
    }else{

        outb(ATA_COMMAND, 0xC8);
    }

    // set the start stop bit
    if(write){
        outb(bar4, 1);
    }else{
        outb(bar4, (8 | 1));
    }
    while (irq_received == false);
    memcpy(buf, 0xFF410000, 512);
    
}

void ata_drive(bool write, int addr, int slavebit, unsigned char* buffer){
    
    // BUSY check
    ATA_wait_BSY();

    outb(ATA_DRIVE, 0xE0 | (slavebit << 4) | ((addr >> 24) & 0x0F));
    outb(ATA_FEATURES, 0x00);
    int sector_count = 1; 
    outb(ATA_SECTOR_COUNT, (unsigned char)sector_count);
    outb(ATA_SECTOR_NUM, (unsigned char) addr);
    outb(ATA_LOW, (unsigned char) (addr>> 8));
    outb(ATA_HIGH, (unsigned char) (addr >> 16));



    if(write){

        outb(ATA_COMMAND, 0x30);
    }else{

        outb(ATA_COMMAND, 0x20);
    }
    delay_400ns();
    // poll


    if(write){
        ATA_wait_BSY();
        ATA_wait_DRQ();
        for(int i = 0; i < 512; i += 2){
            //(buffer[i] >> 8) & buffer[i] 

            outsw(ATA_DATA, (buffer[i] + (buffer[i + 1] << 8)));

        }

        // Send CACHE FLUSH command
        outb(ATA_COMMAND, 0xE7);

    }else{
        ATA_wait_BSY();
        ATA_wait_DRQ();
        asm ( "rep insw" // Input words from I/O port 
        : // No output operands
        : "d" (ATA_DATA), "D" (buffer), "c" (256) // ES:DI points to destination, ECX specifies count 
        );
        // uint16_t data;
        // for(int i = 0; i < 512; i += 2){
        //     //print16(insw(ATA_DATA));
        //     //printf(" ");
        //     //memcpy(buffer, tb, 512);
        //     data = insw(ATA_DATA);
        //     // //print16(data);
        //     // //printf(" ");
        //     buffer[i] = (data & 0xFF);
        //     buffer[i + 1] = (data >> 8);
        // }
    }
    delay_400ns();
}


void ata_read(int slave_bit, uint32_t LBA, unsigned char* buffer){
ata_drive(false, LBA, slave_bit, buffer);
return;
    if(drive){
        ata_dma_drive(false, LBA, slave_bit, buffer);
    }else{
        ata_drive(false, LBA, slave_bit, buffer);
    }

}

unsigned char* buffer;

void ata_write(int slave_bit, uint32_t LBA, unsigned char* buffer){
        ata_drive(true, LBA, slave_bit, buffer);
        return;
    if(drive){
        ata_dma_drive(true, LBA, slave_bit, buffer);
    }else{
        ata_drive(true, LBA, slave_bit, buffer);
    }
}

void ata_handler(struct regs *r){
    //printf("ATA INTERRUPT\n");'

    if(drive){
        outb(bar4, 0); // clear start/stop bit
        //printf("DMA COMPLETE STATUS %X\n", inb(bar4 + 2));
            irq_received = true;
    }
}

void ata_init(){
    printf("initializing drive...\n");
    outb(ATA_DRIVE, 0xA0);
    // set all to zero
    outb(ATA_SECTOR_COUNT, 0);
    outb(ATA_SECTOR_NUM, 0);
    outb(ATA_LOW, 0);
    outb(ATA_HIGH, 0);

    // send IDENTIFY
    outb(ATA_COMMAND, 0xEC);

    int status = inb(ATA_STATUS);
    if(status == 0){
        printf("NO DRVIE FOUND\n");
        return;
    }else{
        printf("Checking drive...\n");
        // check busy bit
        for(int i = 0; i < 4; i++){
            while((status & 0x80) != 0){
                status = inb(ATA_STATUS);
            }
        }       
        printf("Drive found...\n");
        status = inb(ATA_LOW);
        if(status != 0){
            printf("ERROR NOT ATA DRIVE\n");
        }
        status = inb(ATA_HIGH);
        if(status != 0){
            printf("ERROR NOT ATA DRIVE\n");
        }

        status = inb(ATA_STATUS);
        for(int i =0 ; i < 4; i++){
            while ((status & 0x08) == 0)
            {
                status = inb(ATA_STATUS);
                if((status & 1) != 0){
                    printf("ERROR\n");
                }
            }
        }
        
        for(int i = 0; i < 256; i++){
            insw(ATA_DATA);
        }
        
        pci_device_t* dev = pci_dev_root;
        while (dev != NULL)
        {

            if(dev->baseclass == 0x1 && dev->subclass == 0x1
            && dev->lf == 0x80){
                drive = dev;
                break;
            }
            dev = dev->next;
        }
        // configure DMA
        if(drive){
            if(get_header_type(drive->bus, drive->device, dev->function) == 0){
                unsigned short c = pci_config_read_word(drive->bus, drive->device, dev->function, 4);
                pci_config_write_word(drive->bus, drive->device, dev->function, 4, c | 0x4);

                bar4 =  pci_config_read_word(drive->bus, drive->device, drive->function, 34);
                bar4 = bar4 << 16;
                bar4 |= pci_config_read_word(drive->bus, drive->device, drive->function, 32);
                if((bar4 & 1)){
                    printf("DMA ");
                    bar4 = bar4 & 0xFFFFFFFC;
                    prdt[0] = 0x80010000; 
                    prdt[0] |= (512 << 32);
                    prdt[0] |= 0x8000000000000000; // set msb

                    outb(bar4 + 0x4, ((unsigned int)0x80000000 & 0xFF));
                    outb(bar4 + 0x5, (((unsigned int)0x80000000 & 0xFF00) >> 8));
                    outb(bar4 + 0x6, (((unsigned int)0x80000000 & 0xFF0000) >> 16));
                    outb(bar4 + 0x7, (((unsigned int)0x80000000 & 0xFF000000) >> 24));
                }

            }
        }

        printf("DRIVE LOADED\n");
        drive_loaded = true;
        irq_received = false;
        // disable IRQ
        //outb(ATA_CONTROL_BASE, 0b00000010);
    }
}