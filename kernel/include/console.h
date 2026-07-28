#pragma once

#include <stddef.h>
#include <stdint.h>
#include "vga.h"
#include "io.h"
#include "string.h"

#define MAX_CONSOLE_LEN 128

// void set_command_info(const char* name, const char* description, command_info command_info);
void update_cursor();
void vga_scroll();
void console_clear();
void console_process_key(char key);
void console_command_handler(const char* command);
void print_start_symbol();
void set_color_info();
void dispatch_interrupt(int vector);