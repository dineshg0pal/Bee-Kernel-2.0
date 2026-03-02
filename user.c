#include <stdint.h>

static inline int syscall(int num, int arg)
{
    int ret;
    __asm__ volatile (
        "int $0x80"
        : "=a"(ret)
        : "a"(num), "b"(arg)
    );
    return ret;
}

void sys_print(const char* str)
{
    syscall(1, (int)str);
}

char sys_getchar()
{
    return syscall(2, 0);
}

void user_main()
{
    sys_print("bee$ ");

    while (1)
    {
        char c = sys_getchar();
        if (!c)
            continue;

        // ENTER
        if (c == '\n')
        {
            sys_print("\nbee$ ");
            continue;
        }

        // BACKSPACE
        if (c == 8)
        {
            sys_print("\b \b");  
            continue;
        }

        // NORMAL CHAR
        char str[2] = {c, 0};
        sys_print(str);
    }
}
