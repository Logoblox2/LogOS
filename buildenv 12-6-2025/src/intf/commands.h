#pragma once

// Command handler type
typedef void (*command_func)(const char* args);

// Register and process commands
void process_command(const char* input);