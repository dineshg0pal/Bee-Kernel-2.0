#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

/* Initialize keyboard (enable PS/2 translation) */
void keyboard_init();

/* Returns 0 if no key pressed, otherwise ASCII character */
char keyboard_getchar();

/* IRQ handler called from IDT */
void irq1_handler();

#endif
