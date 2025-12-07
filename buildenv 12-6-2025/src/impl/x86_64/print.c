#include "print.h"
#include <stdint.h>
#include "io.h"

// VGA text mode buffer starts at 0xB8000
static uint16_t* video_memory = (uint16_t*)0xB8000;

// Cursor position
static uint8_t cursor_row = 0;
static uint8_t cursor_col = 0;

// Current color
static uint8_t print_color = (PRINT_COLOR_LIGHT_GRAY | (PRINT_COLOR_BLACK << 4));

// --- Low-level cursor update ---
static void move_cursor(int row, int col) {
    uint16_t pos = row * 80 + col;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

// --- Clear screen ---
void print_clear() {
    for (int y = 0; y < 25; y++) {
        for (int x = 0; x < 80; x++) {
            video_memory[y * 80 + x] = (uint16_t)(' ' | (print_color << 8));
        }
    }
    cursor_row = 0;
    cursor_col = 0;
    move_cursor(cursor_row, cursor_col);
}

// --- Set color ---
void print_set_color(uint8_t foreground, uint8_t background) {
    print_color = foreground | (background << 4);
}

// --- Print single character ---
void print_char(char c) {
    if (c == '\n') {
        cursor_row++;
        cursor_col = 0;
    } else if (c == '\b') {
        if (cursor_col > 0) {
            cursor_col--;
            video_memory[cursor_row * 80 + cursor_col] = (uint16_t)(' ' | (print_color << 8));
        }
    } else {
        video_memory[cursor_row * 80 + cursor_col] = (uint16_t)(c | (print_color << 8));
        cursor_col++;
        if (cursor_col >= 80) {
            cursor_col = 0;
            cursor_row++;
        }
    }

    if (cursor_row >= 25) {
        cursor_row = 24; // stay at bottom
    }
    move_cursor(cursor_row, cursor_col);
}

// --- Print string ---
void print_str(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        print_char(str[i]);
    }
}

// --- Backspace helper ---
void print_backspace() {
    print_char('\b');
}