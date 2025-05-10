#include <stdint.h>
#include "io.h"
#include "ata_pio_read.h"

void ata_wait(){
    while(inb(ATA_PRIMARY_IO + 7) & ATA_STATUS_BUSY);
}

void ata_pio_read28(uint32_t lba, uint8_t sector_count, void* buf) {
    ata_wait();

    outb(ATA_PRIMARY_IO + 1, 0x00);
    outb(ATA_PRIMARY_IO + 2, sector_count);
    outb(ATA_PRIMARY_IO + 3, (uint8_t) lba);
    outb(ATA_PRIMARY_IO + 4, (uint8_t) lba >> 8);
    outb(ATA_PRIMARY_IO + 5, (uint8_t) lba >> 16);

    outb(ATA_PRIMARY_IO + 6, 0xE0 | ((lba >> 24) & 0x0F));
    outb(ATA_PRIMARY_IO + 7, ATA_CMD_READ_PIO);

    uint16_t* target = (uint16_t*)buf;

    for(int i = 0; i < sector_count; i++){
        ata_wait();
        while(!(inb(ATA_PRIMARY_IO + 7) & ATA_STATUS_DRQ));

        for(int j = 0; j < 256; j++){
            target[j] = inw(ATA_PRIMARY_IO);
        }
        target += 256;
    }
}