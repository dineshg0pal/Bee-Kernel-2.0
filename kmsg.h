#ifndef KMSG_H
#define KMSG_H

void kmsg_init();
void kmsg_write(const char* msg);
const char* kmsg_get_buffer();

#endif
