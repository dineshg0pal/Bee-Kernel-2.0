// kernel.c - Minimal C kernel for GRUB
void kernel_main() {
    char *video = (char *)0xB8000;
    const char *msg = "Hello from Bee Kernel!";
    int i = 0;
    while (msg[i]) {
        video[i*2] = msg[i];      // character
        video[i*2+1] = 0x0F;      // color (white on black)
        i++;
    }
    while (1) {}                  // halt
}
