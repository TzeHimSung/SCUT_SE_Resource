#ifndef Printable_h
#define Printable_h

#include <stdlib.h>

class Print;

/** Printable类为新类提供了一种允许自己进行打印的方式。
    通过从Printable派生并实现printTo方法，用户可以通过将此类的实例传递到通常的Print::print和Print::println方法中来打印出此类的实例。
*/

class Printable {
public:
    virtual size_t printTo(Print &p) const = 0;
};

#endif

