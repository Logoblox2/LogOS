#include "keyboard.h"
#include "print.h"
#include <string.h>

int run_menu(const char* title, const char* options[]) {
    int selected = 0;

    while (1) {
        print_clear();
        print_str(title);
        print_str("\n\n");

        for (int i = 0; options[i] != NULL; i++) {
            if (i == selected) print_str(" > ");
            else print_str("   ");
            print_str(options[i]);
            print_str("\n");
        }

        char c = getch();

        if (c == KEY_UP && selected > 0) selected--;
        else if (c == KEY_DOWN && options[selected+1] != NULL) selected++;
        else if (c == KEY_ENTER) {
            if (strcmp(options[selected], "Back") == 0) return -1;
            return selected;
        } else if (c == KEY_BACKSPACE) {
            return -1;
        }
    }
}

void cmd_config(const char* args) {
    const char* categories[] = {
        "Display Settings",
        "Network Settings",
        "System Info",
        "Exit",
        NULL
    };

    while (1) {
        int choice = run_menu("=== Configuration Menu ===", categories);
        if (choice == -1 || strcmp(categories[choice], "Exit") == 0) break;

        if (strcmp(categories[choice], "Display Settings") == 0) {
            const char* display_opts[] = {
                "Resolution",
                "Color Scheme",
                "Back",
                NULL
            };

            while (1) {
                int dchoice = run_menu("=== Display Settings ===", display_opts);
                if (dchoice == -1 || strcmp(display_opts[dchoice], "Back") == 0) break;

                print_clear();
                print_str("=== Display Settings ===\n\n");
                print_str("Selected: ");
                print_str(display_opts[dchoice]);
                print_str("\n\nPress Enter or Backspace to go back...\n");

                char c;
                do {
                    c = getch();
                } while (c != KEY_ENTER && c != KEY_BACKSPACE);
            }
        }

        else if (strcmp(categories[choice], "Network Settings") == 0) {
            const char* net_opts[] = {
                "WiFi",
                "Ethernet",
                "Back",
                NULL
            };

            while (1) {
                int nchoice = run_menu("=== Network Settings ===", net_opts);
                if (nchoice == -1 || strcmp(net_opts[nchoice], "Back") == 0) break;

                print_clear();
                print_str("=== Network Settings ===\n\n");
                print_str("Selected: ");
                print_str(net_opts[nchoice]);
                print_str("\n\nPress Enter or Backspace to go back...\n");

                char c;
                do {
                    c = getch();
                } while (c != KEY_ENTER && c != KEY_BACKSPACE);
            }
        }

        else if (strcmp(categories[choice], "System Info") == 0) {
            print_clear();
            print_str("\n=== System Info ===\n");
            print_str("\nLogOS v0.01 ALPHA BUILD\n");
            print_str("This build is highly unstable and should not be used AT ALL.\n\n");
            print_str("\nPress any key to return...\n");
            getch();
        }
    }

    print_str("\nExiting configuration menu...\n");
}