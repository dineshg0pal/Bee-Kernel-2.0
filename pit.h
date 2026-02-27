#ifndef PIT_H
#define PIT_H

#include <stdint.h>

void pit_init(uint32_t frequency);
void pit_callback(void);
uint32_t pit_get_ticks();
void pit_sleep(uint32_t milliseconds);

#endif
