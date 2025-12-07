#include "keyboard.h"
#include "io.h"
#include "print.h"
#include "string.h"
#include "commands.h"
#include <stdint.h>

// Modifier states
static int shift_pressed = 0;
static int caps_lock = 0;
static int ctrl_pressed = 0;
static int alt_pressed = 0;

// Input buffer
static char input_buffer[256];
static int buffer_index = 0;

// Normal keymap
static char map[128] = {
    0, 27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n', 0,
    'a','s','d','f','g','h','j','k','l',';','\'','`', 0,
    '\\','z','x','c','v','b','n','m',',','.','/', 0,
    '*', 0,' ',
};

// Shifted keymap
static char shift_map[128] = {
    0, 27, '!','@','#','$','%','^','&','*','(',')','_','+', '\b',
    '\t','Q','W','E','R','T','Y','U','I','O','P','{','}','\n', 0,
    'A','S','D','F','G','H','J','K','L',':','"','~', 0,
    '|','Z','X','C','V','B','N','M','<','>','?', 0,
    '*', 0,' ',
};

// Read scancode from keyboard controller
static uint8_t keyboard_read_scancode() {
    while ((inb(0x64) & 0x01) == 0);
    return inb(0x60);
}

// Handle modifier keys
static void handle_modifier(uint8_t scancode, int pressed) {
    switch (scancode) {
        case 0x2A: // Left Shift
        case 0x36: // Right Shift
            shift_pressed = pressed;
            break;
        case 0x1D: // Ctrl
            ctrl_pressed = pressed;
            break;
        case 0x38: // Alt
            alt_pressed = pressed;
            break;
        case 0x3A: // Caps Lock (toggle)
            if (pressed) caps_lock = !caps_lock;
            break;
    }
}

// Convert scancode to ASCII
static char scancode_to_ascii(uint8_t scancode) {
    if (scancode >= 128) return 0;
    char c = (shift_pressed ^ caps_lock) ? shift_map[scancode] : map[scancode];
    return c;
}

// Handle input characters
static void handle_input_char(char c) {
    if (c == '\n') {
        input_buffer[buffer_index] = '\0'; // Null terminate
        print_str("\n");
        process_command(input_buffer);     // Dispatch to commands module
        buffer_index = 0;                  // Reset buffer
        print_str("> ");                   // Prompt
    } else if (c == '\b') {
        if (buffer_index > 0) {
            buffer_index--;
            print_backspace();
        }
    } else {
        if (buffer_index < sizeof(input_buffer) - 1) {
            input_buffer[buffer_index++] = c;
            print_char(c);
        }
    }
}

// Main keyboard loop
void keyboard_wait_and_print() {
    print_str("> "); // Initial prompt
    while (1) {
        uint16_t scancode = keyboard_read_scancode();
        if (scancode == 0) continue;

        int pressed = !(scancode & 0x80);
        uint8_t code = scancode & 0x7F;

        handle_modifier(code, pressed);

        if (!pressed) continue;

        char c = scancode_to_ascii(code);
        if (c) {
            handle_input_char(c);
        }
    }
}