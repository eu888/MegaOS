#include <stdint.h>
#include <stddef.h>

#define ISO_SECTOR_SIZE 2048
#define ISO_PRIMARY_VD_OFFSET 0x800
#define ISO_VD_PRIMARY_TYPE 0x01
#define ISO_PATH_TABLE_LOCATION_OFFSET 0x10

typedef struct {
    uint8_t type;
    uint8_t id[5];
    uint8_t version[2];
    uint8_t flags;
    uint8_t system_id[32];
    uint8_t volume_id[32];
    uint8_t reserved[8];
    uint32_t volume_space_size;
    uint16_t volume_set_size;
    uint16_t volume_sequence_number;
    uint16_t logical_block_size;           
    uint32_t path_table_size;             
    uint32_t path_table_location;          
    uint32_t optional_path_table_location;
    uint8_t reserved2[164];
} __attribute__((packed)) iso_volume_descriptor_t;

typedef struct{
    uint8_t length;
    uint8_t ext_attr_length;
    uint32_t lba;
    uint16_t length_of_name;
    uint8_t name[1];
} __attribute__((packed)) iso_directory_record_t;

void* read_sector(uint32_t lba, void* buffer);
iso_volume_descriptor_t* read_primary_volume_descriptor(uint32_t lba);
void* read_path_table(uint32_t lba);
iso_directory_record_t* read_directory_record(uint32_t lba, uint32_t offset);
void* read_file_from_iso(const char* filename, uint32_t root_dir_lba);