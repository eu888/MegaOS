#include "console.h"

volatile uint16_t* video_memory = (uint16_t*)VGA_MEMORY;

size_t cursor_x = 0;
size_t cursor_y = 0;
extern uint8_t terminal_color;
char console_command_buffer[MAX_CONSOLE_LEN];
char buffer[2];
uint8_t command_length = 0;

/**
 * @brief Updates the position of the cursor on the screen.
 */
void update_cursor(){
    uint16_t pos = cursor_y * VGA_WIDTH + cursor_x;
    outb(0x3D4, 0xF);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0xE);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

/**
 * @brief Scrolls the VGA text buffer.
 */
void vga_scroll(){
    for(size_t y = 1;y < VGA_HEIGHT;y++){
        for(uint8_t x = 0;x < VGA_WIDTH;x++){
            video_memory[(y - 1) * VGA_WIDTH + x] = video_memory[y * VGA_WIDTH + x];
        }
    }
    for(size_t x = 0;x < VGA_WIDTH;x++){
        video_memory[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = (terminal_color << 8) | ' ';
    }
    if(cursor_y > 0) cursor_y--;
}

/**
 * @brief Clears the console/terminal screen and resets cursor.
 */
void console_clear(){
    terminal_clear();
    cursor_x = 0;
    cursor_y = 0;
    command_length = 0;
    update_cursor();
}

/**
 * @brief Processes a key press event in the console.
 * @param key The key that was pressed.
 */
void console_process_key(char key){
    if(key == '\n' || key == '\r'){
        console_command_buffer[command_length] = '\0';
        console_command_handler(console_command_buffer);
        command_length = 0;
        print_start_symbol();
    } else if (key == '\b'){
        if (command_length > 0) {
            command_length--;
        }
    } else{
        if(command_length >= MAX_CONSOLE_LEN) return;
        console_command_buffer[command_length++] = key;
    }
}

/**
 * @brief The actual handler for commands. Contains the code to execute all the console/terminal commands.
 * @param command The line which the command and it`s params are.
 */
void console_command_handler(const char* command){
    if(strncmp(command, "echo", 4) == 0){
        terminal_write_string(command+5);
        terminal_put_char('\n');
    } else if(strcmp(command, "clear") == 0){
        console_clear();
    } else if(strncmp(command, "color", 5) == 0){
        if(command_length <= 5){
            set_color_info();
            return;
        }
        command+=6;
        uint8_t bg = 0;
        uint8_t fg = 0;
        while(*command != ' '){
            bg = bg * 10 + (*command - '0');
            command++;
        }
        command++;
        while(*command != '\n' && *command != '\0'){
            fg = fg * 10 + (*command - '0');
            command++;
        }
        if((bg > 15) || (fg > 15)){
            set_color_info();
            return;
        }
        terminal_color = fg | bg << 4;
    } else if (strncmp(command, "int", 3) == 0){
        if(command_length <= 4){
            terminal_write_string("Use: int <number>.\n");
            return;
        }
        command+=4;
        uint8_t int_nr = 0;
        while(*command != '\n' && *command != '\0'){
            int_nr = int_nr * 10 + (*command - '0');
            command++;
        }
        if(int_nr > 255){
            terminal_write_string("Invalid interrupt number. Must be between 0 and 255.\n");
            return;
        }
        
        dispatch_interrupt(int_nr);

    } else{
        terminal_write_string("Command ");
        terminal_write_string(command);
        terminal_write_string(" was not found.\n");
    }
}

/**
 * @brief Prints conslole/terminal start symbol.
 */
void print_start_symbol(){
    terminal_write_string("> ");
}

/**
 * @brief Sets the color information for the terminal.
 */
void set_color_info(){
    terminal_write_string("Use: color <background> <foreground>.\n");
    terminal_write_string("BLACK = 0\t\t DARK_GREY = 8\nBLUE = 1\t\t  LIGHT_BLUE = 9\nGREEN = 2\t\t LIGHT_GREEN = 10\nCYAN = 3\t\t  LIGHT_CYAN = 11\nRED = 4\t\t   LIGHT_RED = 12\nMAGENTA = 5\t   LIGHT_MAGENTA = 13\nBROWN = 6\t\t LIGHT_BROWN = 14,\nLIGHT_GREY = 7\tWHITE = 15\n");
}


/**
 * @brief Dispatches an interrupt based on its vector.
 * @param vector The interrupt vector number.
 */
void dispatch_interrupt(int vector){
    switch(vector){
        case 0x00: asm volatile("int $0x00"); break;
        case 0x08: asm volatile("int $0x08"); break;
        case 0x0D: asm volatile("int $0x0D"); break;
        case 0x0E: asm volatile("int $0x0E"); break;
        case 0x20: asm volatile("int $0x20"); break;
        case 0x21: asm volatile("int $0x21"); break;
        default: terminal_write_string("Interrupt not implemented.\n");
    }
}