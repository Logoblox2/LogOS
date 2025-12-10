#include "print.h"
#include "commands.h"
#include <string.h>
#include "io.h"
#include "config.h"
#include "ata.h"

// --- Handlers ---
void cmd_help(const char* args) {
    print_str("\nAvailable commands:\n");
    for (int i = 0; i < num_commands; i++) {
        print_str("  ");

        int has_names = 0;
        for (int j = 0; commands[i].names[j] != NULL; j++) {
            if (j > 0) print_str("/");
            print_str(commands[i].names[j]);
            has_names = 1;
        }

        if (has_names) {
            print_str(" - ");
        }

        print_str(commands[i].description);
        print_str("\n");
    }
    print_str("\n");
}

// --- Commands ---
void cmd_clear(const char* args) {
    print_clear();
}

void cmd_echo(const char* args) {
    if (!args || !*args) { print_str("Usage: echo <text>\n"); return; }
    print_str(args);
    print_str("\n");
}

void cmd_ls(const char* args) {
    print_str("This command is a work in progress!\n");
}

void cmd_cd(const char* args) {
    if (!args || !*args) { print_str("Usage: cd <dir>\n"); return; }
    print_str("This command is a work in progress!\n");
}

void cmd_mkfile(const char* args) {
    if (!args || !*args) { print_str("Usage: mkfile <file name>\n"); return; }
    print_str("This command is a work in progress!\n");
}

void cmd_mkdir(const char* args) {
    if (!args || !*args) { print_str("Usage: mkdir <file name>\n"); return; }
    print_str("This command is a work in progress!\n");
}

void cmd_shutdown(const char* args) {
    print_str("Shutting down...\n");
    outw(0x604, 0x2000);
}

void cmd_write(const char* args) {
    if (!args || !*args) {
        print_str("Usage: write <text>\n");
        return;
    }

    uint8_t buf[512];
    memset(buf, 0, sizeof(buf));
    memcpy(buf, args, strlen(args));
    buf[strlen(args)] = '\0';

    if (ata_write_sector(0x100, buf) == 0) {
        print_str("Wrote to sector 0x100.\n");
    } else {
        print_str("Write failed.\n");
    }
}

void cmd_read(const char* args) {
    (void)args;
    uint8_t buf[512];
    if (ata_read_sector(0x100, buf) == 0) {
        print_str("Read back: ");
        print_str((char*)buf);
        print_str("\n");
    } else {
        print_str("Read failed.\n");
    }
}

// --- Command table ---
struct command_entry commands[] = {
    { { "help", NULL }, "Show this help message", cmd_help },
    { { "clear", "clr", "cls", NULL }, "Clear the screen", cmd_clear },
    { { "echo", "print", NULL }, "Print text to the screen", cmd_echo },
    { { "shutdown", "poweroff", "halt", NULL }, "Shuts down the system", cmd_shutdown },
    { { "config", "conf", "configuration", NULL }, "Open configuration menu", cmd_config },
    { { NULL }, "--- TESTING ---"},
    { { "write", NULL }, "Write text to sector 0x100", cmd_write },
    { { "read", NULL }, "Read text from sector 0x100", cmd_read },
    { { NULL }, "--- WORK IN PROGRESS ---"},
    { { "ls", "dir", NULL }, "Lists the files in the directory you're currently in", cmd_ls },
    { { "cd", NULL }, "Moves to the specified directory", cmd_cd },
    { { "mkfile", NULL }, "Creates a file with the specified name", cmd_mkfile },
    { { "mkdir", NULL }, "Creates a directory with the specified name", cmd_mkdir },
};

const int num_commands = sizeof(commands) / sizeof(commands[0]);