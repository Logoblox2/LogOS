#pragma once

void keyboard_wait_and_print();

char getch(void);

#define KEY_ENTER     '\n'
#define KEY_BACKSPACE 0x08
#define KEY_ESC       0x1B

#define KEY_UP        1
#define KEY_DOWN      2
#define KEY_LEFT      3
#define KEY_RIGHT     4