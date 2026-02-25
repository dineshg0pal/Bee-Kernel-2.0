#include "shell.h"
#include "vga.h"

#define SHELL_BUFFER_SIZE 128

static char shell_buffer[SHELL_BUFFER_SIZE];
static int shell_index = 0;

/* ---------- internal helpers ---------- */

static int str_compare(const char* a, const char* b) {
    int i = 0;
    while (a[i] && b[i]) {
        if (a[i] != b[i])
            return 0;
        i++;
    }
    return (a[i] == 0 && b[i] == 0);
}

static void shell_print_prompt() {
    print("bee$ ");
}

static void shell_execute(char* command) {

    if (str_compare(command, "dhelp")) {
        print("\nAvailable commands:\n");
        print("dhelp   - show this help\n");
        print("dclear  - clear screen\n");
        print("dinfo   - kernel information\n");
    }
    else if (str_compare(command, "dclear")) {
        clear_screen();
    }
    else if (str_compare(command, "dinfo")) {
        print("\nBee Kernel v0.1\n");
        print("Simple 32-bit OS\n");
        print("Keyboard + IDT + Shell working\n");
    }
    else {
        print("\nUnknown command: ");
        print(command);
        print("\n");
    }
}

/* ---------- public interface ---------- */

void shell_init() {
    shell_print_prompt();
}

void shell_input(char c) {

    /* ENTER */
    if (c == '\n') {
        shell_buffer[shell_index] = 0;   // terminate string

        shell_execute(shell_buffer);

        shell_index = 0;

        print("\n");
        shell_print_prompt();
        return;
    }

    /* BACKSPACE */
    if (c == '\b') {
        if (shell_index > 0) {
            shell_index--;
            vga_backspace();
        }
        return;
    }

    /* Normal characters */
    if (shell_index < SHELL_BUFFER_SIZE - 1) {
        shell_buffer[shell_index++] = c;
        put_char(c);
    }
}
