#include "print.h"
#include "keyboard.h"
#include "string.h"
#include "commands.h"
#include "ata.h"

void kernel_main() {
    print_clear();
    print_set_color(PRINT_COLOR_WHITE, PRINT_COLOR_BLACK);
    print_str("LogOS has successfully booted!\n");

    uint8_t buf[512] = "Storage sector 0x100 exists and working\n\n";
    ata_write_sector(0x100, buf);

    if (ata_read_sector(0x100, buf) == 0) {
        print_str((char*)buf);
    } else {
        print_str("Read test failed.\n");
    }

    keyboard_wait_and_print();
}

void process_command(const char* input) {
    while (*input == ' ' || *input == '\t') input++;
    if (!*input) return;

    if (!input || !*input) {
        return;
    }

    const char* space = input;
    while (*space && *space != ' ') space++;
    size_t cmd_len = space - input;
    const char* args = (*space) ? space + 1 : "";

    for (int i = 0; i < num_commands; i++) {
        for (int j = 0; commands[i].names[j] != NULL; j++) {
            if (strncmp(input, commands[i].names[j], cmd_len) == 0 &&
                commands[i].names[j][cmd_len] == '\0') {
                commands[i].func(args);
                return;
            }
        }
    }
    
    print_str("Unknown command: ");
    print_str(input);
    print_str("; type 'help' for a list of commands.\n");
}