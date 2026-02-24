#include "vga.h"
#include <stdint.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_ADDRESS 0xB8000

static uint16_t* vga_buffer = (uint16_t*)VGA_ADDRESS;
static uint8_t cursor_row = 0;
static uint8_t cursor_col = 0;

static uint8_t color = 0x0F; // White on black

/* -------------------- I/O PORT -------------------- */
static inline void outb(uint16_t port, uint8_t value) {
    __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

/* -------------------- CURSOR -------------------- */
static void update_cursor() {
    uint16_t pos = cursor_row * VGA_WIDTH + cursor_col;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));

    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

/* -------------------- COLOR -------------------- */
void set_color(uint8_t fg, uint8_t bg) {
    color = (bg << 4) | (fg & 0x0F);
}

/* -------------------- SCROLL -------------------- */
void scroll() {
    for (int row = 1; row < VGA_HEIGHT; row++) {
        for (int col = 0; col < VGA_WIDTH; col++) {
            vga_buffer[(row - 1) * VGA_WIDTH + col] =
                vga_buffer[row * VGA_WIDTH + col];
        }
    }

    for (int col = 0; col < VGA_WIDTH; col++) {
        vga_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + col] =
            (color << 8) | ' ';
    }
}

/* -------------------- CHARACTER OUTPUT -------------------- */
void put_char(char c) {
    if (c == '\n') {
        cursor_col = 0;
        cursor_row++;
        if (cursor_row >= VGA_HEIGHT) {
            scroll();
            cursor_row = VGA_HEIGHT - 1;
        }
        update_cursor();
        return;
    }

    vga_buffer[cursor_row * VGA_WIDTH + cursor_col] = (color << 8) | c;

    cursor_col++;
    if (cursor_col >= VGA_WIDTH) {
        cursor_col = 0;
        cursor_row++;
        if (cursor_row >= VGA_HEIGHT) {
            scroll();
            cursor_row = VGA_HEIGHT - 1;
        }
    }

    update_cursor();
}

/* -------------------- STRING OUTPUT -------------------- */
void print(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        put_char(str[i]);
    }
}

/* -------------------- CLEAR SCREEN -------------------- */
void clear_screen() {
    for (int row = 0; row < VGA_HEIGHT; row++) {
        for (int col = 0; col < VGA_WIDTH; col++) {
            vga_buffer[row * VGA_WIDTH + col] = (color << 8) | ' ';
        }
    }

    cursor_row = 0;
    cursor_col = 0;
    update_cursor();
}

/* -------------------- BACKSPACE -------------------- */
void vga_backspace() {
    if (cursor_col == 0 && cursor_row == 0) return;

    if (cursor_col == 0) {
        cursor_row--;
        cursor_col = VGA_WIDTH - 1;
    } else {
        cursor_col--;
    }

    vga_buffer[cursor_row * VGA_WIDTH + cursor_col] = (color << 8) | ' ';
    update_cursor();
}
