#include <stdint.h>
#include "io.h"
#include "file.h"
#include "print.h"
#include "memory.h"
#include "strings.h"
#include "print.h"
#include "ata_pio_read.h"

static FAT32_BootSector boot_sector;
static uint8_t sector_buffer[512];

void fs_init(uint8_t* buffer, size_t size) {
    print_str("Dumping boot sector:\n");
    for (int i = 0; i < 64; i++) {
        print_hex(buffer[i]);
        print_str(" ");
    }
    print_str("\n");

    FAT32_BootSector* bpb = (FAT32_BootSector*)buffer;
    print_str("OEM: ");
    for (int i = 0; i < 8; i++) {
        print_char(bpb->oem_name[i]);
    }
    print_str("\n");

    print_str("Bytes per sector: ");
    print_hex(bpb->bytes_per_sector);
    print_str("\n");

    print_str("Sectors per cluster: ");
    print_hex(bpb->sectors_per_cluster);
    print_str("\n");
}


void fs_read(uint32_t lba, uint8_t sector_count, void* buf) {
    ata_pio_read28(lba, sector_count, buf);
}

void fs_write(uint32_t lba, uint8_t sector_count, void* buf) {
    // Write data to the filesystem
    print_str("Writing data to filesystem...\n");
    // Implement write logic here
}

void fs_format(uint8_t* disk, uint32_t disk_size) {
    // Format the filesystem
    print_str("Formatting filesystem...\n");
    // Implement format logic here
}

void fs_open(const char* filename) {
    uint32_t root_dir_lba = boot_sector.reserved_sector_count +
        (boot_sector.num_fats * boot_sector.sectors_per_fat);
    fs_read(root_dir_lba, boot_sector.sectors_per_cluster, sector_buffer);

    FAT32_DirEntry* entries = (FAT32_DirEntry*)sector_buffer;
    int found = 0;
    for (int i = 0; i < 16; i++) { 
        if (entries[i].name[0] == 0x00) break; 
        if ((entries[i].attr & 0x0F) == 0x0F) continue; 
        if (strncmp(entries[i].name, filename, 11) == 0) {
            print_str("File found: ");
            print_str(filename);
            print_str(", size: ");
            print_hex(entries[i].file_size);
            print_str(" bytes\n");
            found = 1;
            break;
        }
    }
    if (!found) {
        print_str("File not found: ");
        print_str(filename);
        print_str("\n");
    }
}

void fs_close(const char* filename) {
    // Close a file in the filesystem
    print_str("Closing file: ");
    print_str(filename);
    print_str("\n");
}

void fs_list_directory(const char* path) {
    (void)path; // Only root directory supported
    uint32_t first_data_sector = boot_sector.reserved_sector_count +
    (boot_sector.num_fats * boot_sector.sectors_per_fat);

    uint32_t root_cluster = boot_sector.root_cluster;
    uint32_t root_dir_lba = first_data_sector + ((root_cluster - 2) * boot_sector.sectors_per_cluster);

    FAT32_DirEntry* entries = (FAT32_DirEntry*)sector_buffer;
    print_str("Root directory:\n");
    for (int i = 0; i < 16; i++) {
        if (entries[i].name[0] == 0x00) break;
        if ((entries[i].attr & 0x0F) == 0x0F) continue;
        print_str(" - ");
        for (int j = 0; j < 11; j++) {
            print_char(entries[i].name[j]);
        }
        print_str("  Size: ");
        print_hex(entries[i].file_size);
        print_str("\n");
    }
}