#include "fs.h"
#include "vga.h"
#include "klog.h"

/* ================= RAMFS ================= */

#define MAX_FILES 8
#define MAX_FILE_SIZE 256

typedef struct {
    char name[32];
    uint8_t data[MAX_FILE_SIZE];
    uint32_t size;
    uint8_t used;
} file_t;

static file_t ramfs[MAX_FILES];

/* init */
void fs_init()
{
    for (int i = 0; i < MAX_FILES; i++)
        ramfs[i].used = 0;

    klog("RAMFS initialized");
}

/* create */
int fs_create(const char* name)
{
    for (int i = 0; i < MAX_FILES; i++)
    {
        if (!ramfs[i].used)
        {
            ramfs[i].used = 1;

            int j = 0;
            while (name[j] && j < 31) {
                ramfs[i].name[j] = name[j];
                j++;
            }
            ramfs[i].name[j] = 0;

            ramfs[i].size = 0;
            return 1;
        }
    }
    return 0;
}

/* write */
int fs_write(const char* name, const char* data)
{
    for (int i = 0; i < MAX_FILES; i++)
    {
        if (ramfs[i].used)
        {
            int j = 0, match = 1;

            while (name[j] || ramfs[i].name[j])
            {
                if (name[j] != ramfs[i].name[j]) {
                    match = 0;
                    break;
                }
                j++;
            }

            if (match)
            {
                int k = 0;
                while (data[k] && k < MAX_FILE_SIZE)
                {
                    ramfs[i].data[k] = data[k];
                    k++;
                }
                ramfs[i].size = k;
                return 1;
            }
        }
    }
    return 0;
}

/* read */
void fs_read(const char* name)
{
    for (int i = 0; i < MAX_FILES; i++)
    {
        if (ramfs[i].used)
        {
            int j = 0, match = 1;

            while (name[j] || ramfs[i].name[j])
            {
                if (name[j] != ramfs[i].name[j]) {
                    match = 0;
                    break;
                }
                j++;
            }

            if (match)
            {
                for (uint32_t k = 0; k < ramfs[i].size; k++)
                {
                    char c[2] = {ramfs[i].data[k], 0};
                    print(c);
                }
                print("\n");
                return;
            }
        }
    }

    print("File not found\n");
}

/* list */
void fs_ls()
{
    print("Files:\n");

    for (int i = 0; i < MAX_FILES; i++)
    {
        if (ramfs[i].used)
        {
            print("- ");
            print(ramfs[i].name);
            print("\n");
        }
    }
}

int fs_delete(const char* name)
{
    for (int i = 0; i < MAX_FILES; i++)
    {
        if (ramfs[i].used)
        {
            int j = 0, match = 1;

            while (name[j] || ramfs[i].name[j])
            {
                if (name[j] != ramfs[i].name[j]) {
                    match = 0;
                    break;
                }
                j++;
            }

            if (match)
            {
                ramfs[i].used = 0;
                ramfs[i].size = 0;
                return 1;
            }
        }
    }
    return 0;
}

int fs_append(const char* name, const char* data)
{
    for (int i = 0; i < MAX_FILES; i++)
    {
        if (ramfs[i].used)
        {
            int j = 0, match = 1;

            while (name[j] || ramfs[i].name[j])
            {
                if (name[j] != ramfs[i].name[j]) {
                    match = 0;
                    break;
                }
                j++;
            }

            if (match)
            {
                uint32_t k = ramfs[i].size;
                int d = 0;

                while (data[d] && k < MAX_FILE_SIZE)
                {
                    ramfs[i].data[k++] = data[d++];
                }

                ramfs[i].size = k;
                return 1;
            }
        }
    }
    return 0;
}
