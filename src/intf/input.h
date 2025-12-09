#ifndef INPUT_H
#define INPUT_H

// Reads a single character from keyboard
char getch(void);

// Key constants (adjust for your environment)
#define KEY_ENTER     '\n'
#define KEY_BACKSPACE 0x08
#define KEY_ESC       0x1B

// Arrow keys often come as escape sequences, so you may need parsing
#define KEY_UP        1
#define KEY_DOWN      2
#define KEY_LEFT      3
#define KEY_RIGHT     4

#endif