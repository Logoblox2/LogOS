// commands.h
#ifndef COMMANDS_H
#define COMMANDS_H

typedef void (*command_func)(const char* args);

struct command_entry {
    const char* names[4];     // up to 3 aliases + NULL terminator
    const char* description;
    command_func func;
};

// Expose the command table and dispatcher
extern struct command_entry commands[];
extern const int num_commands;

void process_command(const char* input);

#endif