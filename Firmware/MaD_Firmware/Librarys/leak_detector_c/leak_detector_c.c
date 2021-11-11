#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "leak_detector_c.h"

#undef malloc
#undef realloc
#undef calloc
#undef free

static unsigned int memoryUsed = 0;

/*
 * replacement of malloc
 */
void *xmalloc(unsigned int size, const char *file, unsigned int line)
{
	void *ptr = malloc(size);
	void *sizeptr = (void *)(ptr - 8);
	uint16_t size = (*((uint16_t *)sizeptr)) * 16;
	memoryUsed += size;
	//printf("Alloc:%d, Function:%s, Line:%d\n", size, file, line);
	return ptr;
}

void *xrealloc(void *ptr, unsigned int size, const char *file, unsigned int line)
{
	void *newptr = xmalloc(size, file, line);
	if (ptr != NULL)
	{
		if (newptr != NULL)
		{
			memcpy(newptr, ptr, size);
		}
		xfree(ptr, file, line);
	}
	return newptr;
}

/*
 * replacement of calloc
 */
void *xcalloc(unsigned int elements, unsigned int size, const char *file, unsigned int line)
{
	unsigned total_size;
	void *ptr = xmalloc(elements * size, file, line);
	return ptr;
}

/*
 * replacement of free
 */
void xfree(void *mem_ref, const char *file, unsigned int line) //add file/line information
{
	//void *sizeptr = (uint16_t)((uint8_t *)mem_ref - 8); //go to header start
	void *sizeptr = (void *)(mem_ref - 8);
	uint16_t size = (*((uint16_t *)sizeptr)) * 16;
	memoryUsed -= size;
	//printf("Free:%d, Function:%s, Line:%d\n", size, file, line);
	free(mem_ref);
}

/*
 * writes all info of the unallocated memory into a file
 */
void report_mem_leak(void)
{
	printf("MEMORY USED:%d\n", memoryUsed);
}
