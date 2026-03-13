#include <stdint.h>
#include "syscall.h"

/* ====== FORWARD DECLARATIONS ===== */

void sys_print(const char* str);

/* ===== SYSCALL WRAPPERS ===== */

void sys_print(const char* str)
{
    asm volatile(
        "int $0x80"
        :
        : "a"(SYS_PRINT), "b"(str)
    );
}


void sys_spawn()
{
    asm volatile(
        "int $0x80"
        :
        : "a"(SYS_SPAWN)
    );
}

void sys_kill(uint32_t pid)
{
    asm volatile(
        "int $0x80"
        :
        : "a"(SYS_KILL), "b"(pid)
    );
}

void sys_state()
{
    asm volatile(
        "int $0x80"
        :
        : "a"(SYS_STATE)
    );
}


char sys_getchar()
{
    char c;
    asm volatile(
        "int $0x80"
        : "=a"(c)
        : "a"(SYS_GETCHAR)
    );
    return c;
}

void sys_meminfo()
{
    asm volatile(
        "int $0x80"
        :
        : "a"(SYS_MEMINFO)
    );
}

uint32_t sys_alloc()
{
    uint32_t addr;
    asm volatile(
        "int $0x80"
        : "=a"(addr)
        : "a"(SYS_ALLOC)
    );
    return addr;
}

void sys_free(uint32_t addr)
{
    asm volatile(
        "int $0x80"
        :
        : "a"(SYS_FREE), "b"(addr)
    );
}

uint32_t sys_uptime()
{
    uint32_t ticks;

    asm volatile(
        "int $0x80"
        : "=a"(ticks)
        : "a"(SYS_UPTIME)
    );

    return ticks;
}

void sys_kmsg()
{
    asm volatile(
        "int $0x80"
        :
        : "a"(SYS_KMSG)
    );
}

void sys_tasks()
{
    asm volatile(
        "int $0x80"
        :
        : "a"(SYS_TASKS)
    );
}
/* ===== SIMPLE STRING FUNCTIONS ===== */

int strlen(const char* s)
{
    int i = 0;
    while (s[i]) i++;
    return i;
}

int strcmp(const char* a, const char* b)
{
    int i = 0;
    while (a[i] && b[i]) {
        if (a[i] != b[i])
            return 1;
        i++;
    }

    if (a[i] == 0 && b[i] == 0)
        return 0;

    return 1;
}

int strncmp(const char* a, const char* b, int n)
{
    for (int i = 0; i < n; i++)
    {
        if (a[i] != b[i])
            return 1;

        if (a[i] == 0 || b[i] == 0)
            return 0;
    }

    return 0;
}

/* ===== HIVEBOX CONFIG ===== */

#define INPUT_BUFFER 128

static char buffer[INPUT_BUFFER];
static int buf_index = 0;
static uint32_t last_alloc = 0;

/* ===== PRINT PROMPT ===== */

void print_prompt()
{
    sys_print("bee$ ");
}

/* ===== CLEAR BUFFER ===== */

void clear_buffer()
{
    for (int i = 0; i < INPUT_BUFFER; i++)
        buffer[i] = 0;

    buf_index = 0;
}


void to_hex_string(uint32_t n, char* out)
{
    const char* hex = "0123456789ABCDEF";

    out[0] = '0';
    out[1] = 'x';

    for (int i = 0; i < 8; i++) {
        out[9 - i] = hex[n & 0xF];
        n >>= 4;
    }

    out[10] = 0;
}


/* ===== HIVEBOX MAIN ===== */

void user_main()
{
    sys_print("Welcome to Hivebox\n");

    clear_buffer();
    print_prompt();

    while (1)
    {
        char c = sys_getchar();

        /* ===== ENTER PRESSED ===== */
        if (c == '\n')
        {
            sys_print("\n");

            buffer[buf_index] = 0;

            if (buf_index > 0)
            {
                if (strcmp(buffer, "dhelp") == 0)
                {
                    sys_print("Hivebox Commands:\n");
                    sys_print("dhelp       - show commands\n");
                    sys_print("dinfo       - system information\n");
                    sys_print("dclear      - clear screen\n");
                    sys_print("dmem        - memory info\n");
                    sys_print("dalloc      - allocate frame\n");
                    sys_print("dfree       - free last allocated frame\n");
                    sys_print("duptime     - show system uptime\n");
                    sys_print("dspawn      - spawn demo task\n");
                    sys_print("dkill <pid> - terminate task\n");
                    sys_print("dstate      - show scheduler\n");
                }
                else if (strcmp(buffer, "dinfo") == 0)
                {
                    sys_print("HiveOS 32-bit Kernel\n");
                    sys_print("Hivebox v0.1\n");
                    sys_print("Status: Stable\n");
                }
                else if (strcmp(buffer, "dclear") == 0)
                {
                    for (int i = 0; i < 30; i++)
                        sys_print("\n");
                }

                else if (strcmp(buffer, "duptime") == 0)
                {
                    uint32_t t = sys_uptime();

                    sys_print("System uptime (ticks): ");

                    char buf[11];
                    to_hex_string(t, buf);

                    sys_print(buf);
                    sys_print("\n");
                }


                else if (strcmp(buffer, "dmsg") == 0)
                {
                    sys_kmsg();
                }


                else if (strcmp(buffer, "dspawn") == 0)
                {
                    sys_print("Spawning demo task...\n");
                    sys_spawn();
                }


                else if (strncmp(buffer, "dkill ", 6) == 0)
                {
                    uint32_t pid = buffer[6] - '0';

                    sys_kill(pid);

                    sys_print("Kill request sent\n");
                }
 

                else if (strcmp(buffer, "dstate") == 0)
                {
                     sys_state();
                }
               

                else if (strcmp(buffer, "dps") == 0)
                {
                    sys_tasks(); // optional log display
                    // call kernel task list print via syscall
                }


                else if (strcmp(buffer, "dmem") == 0)
                {
                    sys_meminfo();
                }
                else if (strcmp(buffer, "dalloc") == 0)
                {
                    last_alloc = sys_alloc();

                    if (last_alloc == 0)
                    {
                        sys_print("Allocation failed - no free frames\n");
                    }
                    else
                    {
                        sys_print("Allocated frame at: ");
                        char buf[11];
                        to_hex_string(last_alloc, buf);
                        sys_print(buf);
                        sys_print("\n");
                    }
                }
                else if (strcmp(buffer, "dfree") == 0)
                {
                    if (last_alloc != 0)
                    {
                        sys_free(last_alloc);
                        sys_print("Freed frame: ");
                        char buf[11];
                        to_hex_string(last_alloc, buf);
                        sys_print(buf);
                        sys_print("\n");

                        last_alloc = 0;

                    }
                    else
                    {
                        sys_print("No frame allocated\n");
                    }
                }
                else
                {
                    sys_print("Unknown Hive command\n");
                }
            }

            clear_buffer();
            print_prompt();
        }

        /* ===== BACKSPACE ===== */
        else if (c == 8 || c == 14)
        {
            if (buf_index > 0)
            {
                buf_index--;
                buffer[buf_index] = 0;
                sys_print("\b \b");
            }
        }

        /* ===== PRINTABLE CHAR ===== */
        else if (c >= 32 && c <= 126)
        {
            if (buf_index < INPUT_BUFFER - 1)
            {
                buffer[buf_index++] = c;

                char temp[2];
                temp[0] = c;
                temp[1] = 0;

                sys_print(temp);
            }
        }
    }
}
