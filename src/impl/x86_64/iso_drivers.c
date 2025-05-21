#include "iso_driver.h"
#include <stdint.h>
#include <stddef.h>
#include "print.h"
#include "memory.h"
#include "strings.h"

void* read_sector(uint32_t lba, void* buffer) {
    ata_pio_read28(1, lba, buffer);
    return buffer;
}

iso_volume_descriptor_t* read_primary_volume_descriptor(uint32_t lba) {
    iso_volume_descriptor_t* vd = (iso_volume_descriptor_t*)kmalloc(ISO_SECTOR_SIZE);
    read_sector(lba, vd);
    if (vd->type == ISO_VD_PRIMARY_TYPE) {
        return vd;
    }
    return NULL;
}

void* read_path_table(uint32_t lba) {
    void* path_table = (uint8_t)kmalloc(ISO_SECTOR_SIZE);
    read_sector(lba, path_table);
    return path_table;
}

iso_directory_record_t* read_directory_record(uint32_t lba, uint32_t offset) {
    iso_directory_record_t* dir_record = (iso_directory_record_t*)kmalloc(ISO_SECTOR_SIZE);
    read_sector(lba, dir_record);
    return dir_record;
}

void* read_file_from_iso(const char* filename, uint32_t root_dir_lba) {
    iso_directory_record_t* record = read_directory_record(root_dir_lba, 0);

    while(record){
        if(strncmp((char*)record->name, filename, record->length_of_name) == 0){
            uint8_t* file_data = (uint8_t*)kmalloc(ISO_SECTOR_SIZE);
            read_sector(record->lba, file_data);
            return file_data;
        }
        record = (iso_directory_record_t*)((uint8_t*)record + record->length);
    }

    return NULL;
}