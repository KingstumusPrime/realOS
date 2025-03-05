#ifndef _KERNEL_ATA_H
#define _KERNEL_ATA_H
 
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define ATA_IO_BASE 0x1F0
#define ATA_DATA (ATA_IO_BASE + 0)
#define ATA_ERROR (ATA_IO_BASE + 1)
#define ATA_FEATURES (ATA_IO_BASE + 1)
#define ATA_SECTOR_COUNT (ATA_IO_BASE + 2)
#define ATA_SECTOR_NUM (ATA_IO_BASE + 3)
#define ATA_LOW (ATA_IO_BASE + 4)
#define ATA_HIGH (ATA_IO_BASE + 5)
#define ATA_DRIVE (ATA_IO_BASE + 6)
#define ATA_STATUS (ATA_IO_BASE + 7)
#define ATA_COMMAND (ATA_IO_BASE + 7)

#define ATA_CONTROL_BASE 0x3F6
#define ATA_ALT_STATUS (ATA_IO_BASE + 0)
#define ATA_CONTROL (ATA_IO_BASE + 0)
#define ATA_DRIVE_ADDR (ATA_IO_BASE + 1)

void ata_read(int slave_bit, uint32_t LBA, unsigned char* buffer);
void ata_write(int slave_bit, uint32_t LBA, unsigned char* buffer);
void ata_handler(struct regs *r);
void ata_init();
void delay_400ns();


extern bool drive_loaded;
extern volatile bool irq_received;
#endif