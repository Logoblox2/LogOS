#include "print.h"
#include "string.h"
#include "commands.h"

// --- Command Handlers ---
static void cmd_help(const char* args);
static void cmd_clear(const char* args);
static void cmd_echo(const char* args);
static void cmd_vers(const char* args);

// Command table entry with aliases
struct command_entry {
    const char* names[4];     // up to 3 aliases + NULL terminator
    const char* description;
    command_func func;
};

// Table of commands
static struct command_entry commands[] = {
    { { "help", NULL }, "Show this help message", cmd_help },
    { { "clear", "clr", "cls", NULL }, "Clear the screen", cmd_clear },
    { { "echo", NULL }, "Print text to the screen", cmd_echo },
    { { "version", "vers", "ver", NULL }, "Prints the current version of LogOS", cmd_vers },
};

// --- Handlers ---
static void cmd_help(const char* args) {
    print_str("\nAvailable commands:\n");
    for (int i = 0; i < sizeof(commands)/sizeof(commands[0]); i++) {
        print_str("  ");
        // Print all aliases
        for (int j = 0; commands[i].names[j] != NULL; j++) {
            if (j > 0) print_str("/");
            print_str(commands[i].names[j]);
        }
        print_str(" - ");
        print_str(commands[i].description);
        print_str("\n");
    }
    print_str("\n");
}

static void cmd_clear(const char* args) {
    print_clear();
}

static void cmd_echo(const char* args) {
    print_str(args);
    print_str("\n");
}

static void cmd_vers(const char* args) {
    print_str("\nLogOS v0.01 ALPHA BUILD\n");
    print_str("This build is highly unstable and should not be used AT ALL.\n\n");
}

// --- Dispatcher ---
void process_command(const char* input) {
    // Split into command and arguments
    const char* space = input;
    while (*space && *space != ' ') space++;
    size_t cmd_len = space - input;
    const char* args = (*space) ? space + 1 : "";

    for (int i = 0; i < sizeof(commands)/sizeof(commands[0]); i++) {
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
    print_str("\n");
}