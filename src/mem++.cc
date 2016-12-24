#include "hw.hh"
#include <stdio.h>

extern  "C" size_t xFreeBytesRemaining;

void* operator new (unsigned size)
{
	void* p;
    printf_("_get %d", size);
	p = pvPortMalloc(size);
    printf_(", rem %d\n", xFreeBytesRemaining);
	return p;
}
void * operator new[] (unsigned size)
{
    void* p;
    printf_("cxx _get %d", size);
	p = pvPortMalloc(size);
    printf_(", rem %d\n", xFreeBytesRemaining);
	return p;
}
void operator delete (void* p)
{
	vPortFree(p);
    printf_("cxx rem %d\n", xFreeBytesRemaining);
}
void operator delete[] (void* p)
{
	vPortFree(p);
    printf_("cxx rem %d\n", xFreeBytesRemaining);
}

