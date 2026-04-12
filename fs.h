#ifndef FS_H
#define FS_H

#include <stdint.h>

void fs_init();

int fs_create(const char* name);
int fs_write(const char* name, const char* data);
int fs_delete(const char* name);
int fs_append(const char* name, const char* data);
void fs_read(const char* name);
void fs_ls();

#endif
