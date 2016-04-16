#include "MyAssert.h"

#include <cstdio>

void printAssert(const char* str, const char* file, unsigned line)
{
	printf("Assertion failed: %s (in %s:%d)\n", str, file, line);
}