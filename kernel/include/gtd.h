

#include <stdint.h>
#include <stddef.h>
#include "vga.h"
#include "string.h"

typedef struct gdt_entry_bits{
    uint16_t limit_low;
    uint32_t base_low : 24;
    uint8_t accessed : 1;
    uint8_t read_write : 1;
    uint8_t conforming_expand_down : 1;
    uint8_t code : 1;
    uint8_t code_data_segment : 1;
    uint8_t DPL : 2;
    uint8_t present : 1;
    uint8_t limit_high : 4;
    uint8_t available : 1;
    uint8_t long_mode : 1;
    uint8_t big : 1;
    uint8_t granularity : 1;
    uint8_t base_high;
} __attribute__((packed)) gdt_entry_bits_t;