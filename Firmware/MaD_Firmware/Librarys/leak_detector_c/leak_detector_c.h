
#ifndef LEAK_DETECTOR_C_H
#define LEAK_DETECTOR_C_H

#define FILE_NAME_LENGTH 30
#define OUTPUT_FILE "/home/leak_info.txt"
#define malloc(size) xmalloc(size, __FUNCTION__, __LINE__)
#define realloc(ptr, size) xrealloc(ptr, size, __FUNCTION__, __LINE__)
#define calloc(elements, size) xcalloc(elements, size, __FUNCTION__, __LINE__)
#define free(mem_ref) xfree(mem_ref, __FUNCTION__, __LINE__)

void *xmalloc(unsigned int size, const char *file, unsigned int line);
void *xrealloc(void *ptr, unsigned int size, const char *file, unsigned int line);
void *xcalloc(unsigned int elements, unsigned int size, const char *file, unsigned int line);
void xfree(void *mem_ref, const char *file, unsigned int line);

#endif
