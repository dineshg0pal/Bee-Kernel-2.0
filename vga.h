#ifndef VGA_H
#define VGA_H

#include <stdint.h>

void clear_screen();
void print(const char* str);
void put_char(char c);
void set_color(uint8_t fg, uint8_t bg);

/* ---------------- BACKSPACE SUPPORT ---------------- */
void vga_backspace();

#endif
