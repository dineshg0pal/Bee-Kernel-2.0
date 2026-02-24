#ifndef VGA_H
#define VGA_H

#include <stdint.h>

void clear_screen();
void print(const char* str);
void put_char(char c);
void set_color(uint8_t fg, uint8_t bg);
void vga_backspace();
void vga_move_cursor_left();
void vga_move_cursor_right();

#endif
