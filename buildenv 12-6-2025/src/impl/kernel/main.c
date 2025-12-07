#include "print.h"
#include "keyboard.h"

void kernel_main() {
    print_clear();
    print_set_color(PRINT_COLOR_WHITE, PRINT_COLOR_BLACK);
    print_str("LogOS has successfully booted!\n\n");
    keyboard_wait_and_print();
}