#include "print.h"
#include "terminal.h"
#include "io.h"

const static size_t NUM_COLS = VGA_WIDTH;
const static size_t NUM_ROWS = VGA_HEIGHT;

struct Char {
    uint8_t character;
    uint8_t color;
};

struct Char* buffer = (struct Char*) 0xb8000;
size_t col = 0;
size_t row = 0;
uint8_t color = PRINT_COLOR_WHITE | PRINT_COLOR_BLACK << 4;

void clear_row(size_t row) {
    struct Char empty = (struct Char) {
        character: ' ',
        color: color,
    };

    for (size_t col = 0; col < NUM_COLS; col++) {
        buffer[col + NUM_COLS * row] = empty;
    }
}

void print_clear() {
    for (size_t i = 0; i < NUM_ROWS; i++) {
        clear_row(i);
    }
    row = 0;
    col = 0;
}

void print_newline() {
    col = 0;

    if (row < NUM_ROWS - 1) {
        row++;
        return;
    }

    for (size_t row = 1; row < NUM_ROWS; row++) {
        for (size_t col = 0; col < NUM_COLS; col++) {
            struct Char character = buffer[col + NUM_COLS * row];
            buffer[col + NUM_COLS * (row - 1)] = character;
        }
    }

    clear_row(NUM_ROWS - 1);
}

void print_char(char character) {
    if (character == '\n') {
        print_newline();
        return;
    }

    if (character == '\b') {
        if (col > 0) {
            col--;
            buffer[col + NUM_COLS * row] = (struct Char) {
                character: ' ',
                color: color,
            };
        }
        return;
    }

    if (col >= NUM_COLS) {
        print_newline();
    }

    buffer[col + NUM_COLS * row] = (struct Char) {
        character: (uint8_t) character,
        color: color,
    };

    col++;
}

void print_str(char* str) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        char character = (uint8_t) str[i];

        if (character == '\0') {
            return;
        }

        print_char(character);
    }
}

void print_hex(uint64_t value) {
    char hex_digits[] = "0123456789ABCDEF";
    
    print_str("0x");
    for (int i = 60; i >= 0; i -= 4) {
        uint8_t digit = (value >> i) & 0xF;
        print_char(hex_digits[digit]);
    }
}

void print_set_color(uint8_t foreground, uint8_t background) {
    color = foreground + (background << 4);
}

void wait_for_keypress() {
    uint8_t status;
    do {
        status = inb(0x64); // PS/2 status port
    } while (!(status & 1)); // wait until output buffer full
    (void) inb(0x60); // read scancode to clear the buffer
}

void sleep(uint64_t count) {
    for (volatile uint64_t i = 0; i < count; i++) {
        __asm__ volatile("nop");
    }
}

void hexdump(uint8_t* buffer, size_t length) {
    for (size_t i = 0; i < length; i++) {
        uint8_t byte = buffer[i];
        char hex[3] = {
            "0123456789ABCDEF"[byte >> 4],
            "0123456789ABCDEF"[byte & 0xF],
            '\0'
        };
        print_str(hex);
        print_char(' ');

        if ((i + 1) % 16 == 0) {
            print_char('\n');
        }

        if ((i + 1) % 256 == 0) {
            print_str("Pausing...\n");
            sleep(100000000);  // Adjust this count as needed
        }
    }
}
