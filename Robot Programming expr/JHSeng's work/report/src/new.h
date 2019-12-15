#ifndef NEW_H
#define NEW_H

#include <stdlib.h>

void *operator new (size_t size);
void *operator new[](size_t size);
void operator delete (void *ptr);
void operator delete[](void *ptr);

#endif

