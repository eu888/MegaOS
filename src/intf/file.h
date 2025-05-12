#pragma once 

#include <stdint.h>

typedef struct {
    uint8_t  jump_boot[3];         // Jump instruction
    char     oem_name[8];          // OEM Name
    uint16_t bytes_per_sector;     // Bytes per sector
    uint8_t  sectors_per_cluster;  // Sectors per cluster
    uint16_t reserved_sector_count;// Reserved sectors
    uint8_t  num_fats;             // Number of FATs
    uint16_t root_entry_count;     // For FAT12/16 only
    uint16_t total_sectors_16;     // Small volume count
    uint8_t  media;                // Media type
    uint16_t fat_size_16;          // For FAT12/16 only
    uint16_t sectors_per_track;    // BIOS parameter block
    uint16_t num_heads;
    uint32_t hidden_sectors;
    uint32_t total_sectors_32;     // For FAT32

    // FAT32-specific
    uint32_t sectors_per_fat;      // FAT size
    uint16_t ext_flags;
    uint16_t fs_version;
    uint32_t root_cluster;         // Usually 2
    uint16_t fs_info;
    uint16_t backup_boot_sector;
    uint8_t  reserved[12];

    uint8_t  drive_number;
    uint8_t  reserved1;
    uint8_t  boot_signature;
    uint32_t volume_id;
    char     volume_label[11];
    char     fs_type[8];

    uint8_t  boot_code[420];
    uint16_t boot_sector_signature; // 0xAA55
} __attribute__((packed)) FAT32_BootSector;

typedef struct {
    char name[11];
    uint8_t attr;
    uint8_t reserved;
    uint8_t create_time_tenths;
    uint16_t create_time;
    uint16_t create_date;
    uint16_t last_access_date;
    uint16_t high_cluster;
    uint16_t last_modified_time;
    uint16_t last_modified_date;
    uint16_t low_cluster;
    uint32_t file_size;
} __attribute__((packed)) FAT32_DirEntry;


void fs_init(uint8_t* disk, uint32_t disk_size);
void fs_read(uint32_t lba, uint8_t sector_count, void* buf);
void fs_write(uint32_t lba, uint8_t sector_count, void* buf);
void fs_format(uint8_t* disk, uint32_t disk_size);
void fs_open(const char* filename);
void fs_close(const char* filename);
void fs_list_directory(const char* path);