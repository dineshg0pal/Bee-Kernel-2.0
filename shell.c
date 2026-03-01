#include "shell.h"
#include "vga.h"
#include "pmm.h"

#define SHELL_BUFFER_SIZE 128

static char shell_buffer[SHELL_BUFFER_SIZE];
static int shell_index = 0;

static uint32_t last_allocated_frame = 0;

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
        print("dhelp     - show this help\n");
        print("dclear    - clear screen\n");
        print("dinfo     - kernel information\n");
        print("meminfo   - show memory statistics\n");
        print("alloc     - allocate one frame\n");
        print("free      - free last allocated frame\n");
    }

    else if (str_compare(command, "dclear")) {
        clear_screen();
    }

    else if (str_compare(command, "dinfo")) {
        print("\nBee Kernel v0.1\n");
        print("Simple 32-bit OS\n");
        print("Keyboard + IDT + Shell working\n");
    }

    else if (str_compare(command, "meminfo")) {
        print("\nMemory Info:\n");

        print("Total RAM: ");
        print_dec(pmm_get_total_memory() / 1024);
        print(" KB\n");

        print("Used RAM: ");
        print_dec(pmm_get_used_memory() / 1024);
        print(" KB\n");

        print("Free RAM: ");
        print_dec(pmm_get_free_memory() / 1024);
        print(" KB\n");

        print("Total Frames: ");
        print_dec(pmm_get_total_frames());
        print("\n");

        print("Used Frames: ");
        print_dec(pmm_get_used_frames());
        print("\n");
    }

    else if (str_compare(command, "alloc")) {
        last_allocated_frame = pmm_alloc_frame();

        if (last_allocated_frame == 0) {
            print("\nOut of memory!\n");
        } else {
            print("\nAllocated frame at: ");
            print_hex(last_allocated_frame);
            print("\n");
        }
    }

    else if (str_compare(command, "free")) {
        if (last_allocated_frame != 0) {
            pmm_free_frame(last_allocated_frame);
            print("\nFrame freed.\n");
            last_allocated_frame = 0;
        } else {
            print("\nNo frame to free.\n");
        }
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
