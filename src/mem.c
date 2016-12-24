#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
//#include <stdio.h>

/*
void* pvPortMalloc(int size) {
	return malloc(size);
}

void vPortFree(void* ptr) {
	free(ptr);
}
*/

//int putchar(int c) {
//	return 1;
//}

extern  size_t xFreeBytesRemaining;

void* malloc(int size) {
	void* p;
//    printf_("c _get %d", size);
	p = pvPortMalloc(size);
//    printf_(", rem %d\n", xFreeBytesRemaining);
	return p;
}

void free(void* ptr) {
	vPortFree(ptr);
//    printf_("c _rem %d\n", xFreeBytesRemaining);
}

