#include <stdlib.h>

extern "C" void __cxa_pure_virtual(void) __attribute__ ((__noreturn__));
extern "C" void __cxa_deleted_virtual(void) __attribute__ ((__noreturn__));

void __cxa_pure_virtual(void) {
    // 在这种情况下，我们可能要编写一些诊断给uart
    abort();
}

void __cxa_deleted_virtual(void) {
    // 在这种情况下，我们可能要编写一些诊断给uart
    abort();
}

