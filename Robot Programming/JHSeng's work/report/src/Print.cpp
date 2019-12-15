#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "Arduino.h"

#include "Print.h"

// 公共方法 //////////////////////////////////////////////////////////////

/* 默认实现：可能会被覆盖 */
size_t Print::write(const uint8_t *buffer, size_t size) {
    size_t n = 0;
    while (size--) {
        if (write(*buffer++)) n++;
        else break;
    }
    return n;
}

size_t Print::print(const __FlashStringHelper *ifsh) {
    PGM_P p = reinterpret_cast<PGM_P>(ifsh);
    size_t n = 0;
    while (1) {
        unsigned char c = pgm_read_byte(p++);
        if (c == 0) break;
        if (write(c)) n++;
        else break;
    }
    return n;
}

size_t Print::print(const String &s) {
    return write(s.c_str(), s.length());
}

size_t Print::print(const char str[]) {
    return write(str);
}

size_t Print::print(char c) {
    return write(c);
}

size_t Print::print(unsigned char b, int base) {
    return print((unsigned long) b, base);
}

size_t Print::print(int n, int base) {
    return print((long) n, base);
}

size_t Print::print(unsigned int n, int base) {
    return print((unsigned long) n, base);
}

size_t Print::print(long n, int base) {
    if (base == 0) {
        return write(n);
    } else if (base == 10) {
        if (n < 0) {
            int t = print('-');
            n = -n;
            return printNumber(n, 10) + t;
        }
        return printNumber(n, 10);
    } else {
        return printNumber(n, base);
    }
}

size_t Print::print(unsigned long n, int base) {
    if (base == 0) return write(n);
    else return printNumber(n, base);
}

size_t Print::print(double n, int digits) {
    return printFloat(n, digits);
}

size_t Print::println(const __FlashStringHelper *ifsh) {
    size_t n = print(ifsh);
    n += println();
    return n;
}

size_t Print::print(const Printable &x) {
    return x.printTo(*this);
}

size_t Print::println(void) {
    return write("\r\n");
}

size_t Print::println(const String &s) {
    size_t n = print(s);
    n += println();
    return n;
}

size_t Print::println(const char c[]) {
    size_t n = print(c);
    n += println();
    return n;
}

size_t Print::println(char c) {
    size_t n = print(c);
    n += println();
    return n;
}

size_t Print::println(unsigned char b, int base) {
    size_t n = print(b, base);
    n += println();
    return n;
}

size_t Print::println(int num, int base) {
    size_t n = print(num, base);
    n += println();
    return n;
}

size_t Print::println(unsigned int num, int base) {
    size_t n = print(num, base);
    n += println();
    return n;
}

size_t Print::println(long num, int base) {
    size_t n = print(num, base);
    n += println();
    return n;
}

size_t Print::println(unsigned long num, int base) {
    size_t n = print(num, base);
    n += println();
    return n;
}

size_t Print::println(double num, int digits) {
    size_t n = print(num, digits);
    n += println();
    return n;
}

size_t Print::println(const Printable &x) {
    size_t n = print(x);
    n += println();
    return n;
}

// 私有方法 /////////////////////////////////////////////////////////////

size_t Print::printNumber(unsigned long n, uint8_t base) {
    char buf[8 * sizeof(long) + 1]; // Assumes 8-bit chars plus zero byte.
    char *str = &buf[sizeof(buf) - 1];

    *str = '\0';

    // 使用base == 1来防止崩溃
    if (base < 2) base = 10;

    do {
        char c = n % base;
        n /= base;

        *--str = c < 10 ? c + '0' : c + 'A' - 10;
    } while (n);

    return write(str);
}

size_t Print::printFloat(double number, uint8_t digits) {
    size_t n = 0;

    if (isnan(number)) return print("nan");
    if (isinf(number)) return print("inf");
    if (number > 4294967040.0) return print ("ovf");  // 根据经验确定的常数
    if (number < -4294967040.0) return print ("ovf"); // 根据经验确定的常数

    // 处理负数
    if (number < 0.0) {
        n += print('-');
        number = -number;
    }

    // 正确舍入，以便print(1.999，2)打印为“ 2.00”
    double rounding = 0.5;
    for (uint8_t i = 0; i < digits; ++i)
        rounding /= 10.0;

    number += rounding;

    // 提取数字的整数部分并打印
    unsigned long int_part = (unsigned long)number;
    double remainder = number - (double)int_part;
    n += print(int_part);

    // 打印小数点，但仅当位数超出
    if (digits > 0) {
        n += print('.');
    }

    // 一次从其余一位中提取数字
    while (digits-- > 0) {
        remainder *= 10.0;
        unsigned int toPrint = (unsigned int)(remainder);
        n += print(toPrint);
        remainder -= toPrint;
    }

    return n;
}
