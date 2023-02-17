#include <stdbool.h>

#define MAX_SIZE_DATA 255

bool sdcard_open(const char *name, const char *mode);
bool sdcard_close();
bool sdcard_read(const void *mem, int size);
bool sdcard_read_index(const void *mem, int size, int index);
bool sdcard_write(const void *mem, int size);
bool sdcard_begin();
bool sdcard_is_open();