#pragma once

#define ATA_PRIMARY_IO     0x1F0
#define ATA_CMD_READ_PIO   0x20
#define ATA_STATUS_BUSY    0x80
#define ATA_STATUS_DRQ     0x08

void ata_wait();
void ata_pio_read28(uint32_t lba, uint8_t sector_count, void* buf);