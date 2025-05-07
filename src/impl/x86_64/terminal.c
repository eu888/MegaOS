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
const char* buffer_echo = "";
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
        print_str("Available commands: help, clear, info, echo.\n");
        print_str("For more info type help command.\n");
    } else if (strcmp(cmd, "help clear") == 0){
        print_str("Clears the screen.\n");
    } else if (strcmp(cmd, "help info") == 0){
        print_str("Displays info about OS.\n");
    } else if (strcmp(cmd, "help echo") == 0){
        print_str("Writes back the mesage after command.\n");
        print_str("Ex:> echo hello\n");
        print_str("hello\n");
    } else if (strcmp(cmd, "info") == 0) {
        print_str("MegaOS v0.4.1\n");
        print_str("64-bit, stil in progres.\n");
    } else if (strncmp(cmd, "echo", 4) == 0){
        buffer_echo = cmd + 5;
        print_str(buffer_echo);
        print_str("\n");
        buffer_echo = "";
    } else if (strncmp(cmd, "\n", 1) == 0){
        start_simbol();
    }else if(strncmp(cmd, "int", 3)){ 
        buffer_echo = cmd + 4;
        if(strcmp(buffer_echo, "0x35")){
            __asm__ volatile("int $35");
        } else if(strcmp(buffer_echo, "0x32")){
            __asm__ volatile("int $32");
        }
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
        start_simbol();
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

void start_simbol(){
    print_str("> ");
}

