#include	<malloc.h>
#include	"leak_detector_c.h"
int main()
{
	char * ptr1 = (char *)malloc(10);
	int * ptr2 = (int *)calloc(10, sizeof(int));
	float * ptr3 = (float *) calloc(15, sizeof(float));
	free(ptr2);
	atexit(report_mem_leak);
	return 0;
}
