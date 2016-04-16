#pragma once

void printAssert(const char* expression, const char* file, unsigned line);

#define MyAssert(expression) (void)((!!(expression)) || (printAssert(#expression, __FILE__, (unsigned)(__LINE__)), 0))