#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "strings.h"
#include "terminal.h" 
#include "io.h"
#include "print.h"

uint16_t* video_memory = (uint16_t*)0xB8000;

int cursor_x = 0;
int cursor_y = 0;
extern uint8_t color;
char command_buffer[MAX_CMD_LEN];
int command_length = 0; 

void update_cursor() {
    uint16_t pos = cursor_y * VGA_WIDTH + cursor_x;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void scroll_screen() {
    for (int y = 1; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            video_memory[(y - 1) * VGA_WIDTH + x] = video_memory[y * VGA_WIDTH + x];
        }
    }
    for (int x = 0; x < VGA_WIDTH; x++) {
        video_memory[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = (color << 8) | ' ';
    }
    if (cursor_y > 0) cursor_y--;
}

void clear_screen() {
    print_clear();     
    cursor_x = 0;
    cursor_y = 0;
    command_length = 0;
    update_cursor();
}

void handle_command(const char* cmd) {
    if (strcmp(cmd, "clear") == 0) {
        clear_screen();
    } else if (strcmp(cmd, "help") == 0) {
        print_str("Available commands:\n");
        print_str("  help         - Show this help message\n");
        print_str("  clear        - Clear the screen\n");
        print_str("  info         - Display OS information\n");
        print_str("  echo <text>  - Print <text> back to the terminal\n");
        print_str("Type 'help <command>' for more details.\n");
    } else if (strcmp(cmd, "help clear") == 0){
        print_str("clear: Clears the screen and resets the prompt.\n");
    } else if (strcmp(cmd, "help info") == 0){
        print_str("info: Displays information about MegaOS.\n");
    } else if (strcmp(cmd, "help echo") == 0){
        print_str("echo <text>: Writes back the message after the command.\n");
        print_str("Example: > echo hello\n");
        print_str("         hello\n");
    } else if (strcmp(cmd, "info") == 0) {
        print_str("MegaOS v0.5.2\n");
        print_str("64-bit, still in progress.\n");
    } else if (strncmp(cmd, "echo", 4) == 0){
        if (cmd[4] == ' ' && cmd[5] != '\0'){
            print_str(cmd + 5);
        }
        print_str("\n");
    } else if (strcmp(cmd, "\n") == 0 || cmd[0] == '\0') {
        // Do nothing for empty input
    } else {
        print_str("Unknown command: ");
        print_str(cmd);
        print_str("\n");
    }
}

void process_key(char key) {
    if (key == '\n') {
        command_buffer[command_length] = '\0';
        print_char('\n'); 
        handle_command(command_buffer);
        command_length = 0;
        start_symbol();
    } else if (key == '\b') {
        if (command_length > 0) {
            command_length--;
            print_str("\b \b"); 
        }
    } else {
        if (command_length < MAX_CMD_LEN - 1) {
            command_buffer[command_length++] = key;
            print_char(key); 
        }
    }
}

void start_symbol(){
    print_str("> ");
}

