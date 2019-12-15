#ifndef Character_h
#define Character_h
#include <ctype.h>
// 声明常见字符操作，绑定always、inline属性使其变得更强
inline boolean isAlphaNumeric(int c) __attribute__((always_inline));
inline boolean isAlpha(int c) __attribute__((always_inline));
inline boolean isAscii(int c) __attribute__((always_inline));
inline boolean isWhitespace(int c) __attribute__((always_inline));
inline boolean isControl(int c) __attribute__((always_inline));
inline boolean isDigit(int c) __attribute__((always_inline));
inline boolean isGraph(int c) __attribute__((always_inline));
inline boolean isLowerCase(int c) __attribute__((always_inline));
inline boolean isPrintable(int c) __attribute__((always_inline));
inline boolean isPunct(int c) __attribute__((always_inline));
inline boolean isSpace(int c) __attribute__((always_inline));
inline boolean isUpperCase(int c) __attribute__((always_inline));
inline boolean isHexadecimalDigit(int c) __attribute__((always_inline));
inline int toAscii(int c) __attribute__((always_inline));
inline int toLowerCase(int c) __attribute__((always_inline));
inline int toUpperCase(int c) __attribute__((always_inline));
// 检查数字和字母字符
inline boolean isAlphaNumeric(int c) {
    return ( isalnum(c) == 0 ? false : true);
}
// 检查字母字符
inline boolean isAlpha(int c) {
    return ( isalpha(c) == 0 ? false : true);
}
// 检查ASCII字符
inline boolean isAscii(int c) {
    return ( isascii (c) == 0 ? false : true);
}
// 检查空白字符
inline boolean isWhitespace(int c) {
    return ( isblank (c) == 0 ? false : true);
}
// 检查控制字符
inline boolean isControl(int c) {
    return ( iscntrl (c) == 0 ? false : true);
}
// 检查数字
inline boolean isDigit(int c) {
    return ( isdigit (c) == 0 ? false : true);
}
// 检查可打印字符，剔除空格
inline boolean isGraph(int c) {
    return ( isgraph (c) == 0 ? false : true);
}
// 检查小写字母字符
inline boolean isLowerCase(int c) {
    return (islower (c) == 0 ? false : true);
}
// 检查可打印字符，包括空格
inline boolean isPrintable(int c) {
    return ( isprint (c) == 0 ? false : true);
}
// 检查任何非空格的可打印字符或字母数字字符
inline boolean isPunct(int c) {
    return ( ispunct (c) == 0 ? false : true);
}
// 检查空格、换页、换行符、回车、返回、水平制表符和垂直制表符
inline boolean isSpace(int c) {
    return ( isspace (c) == 0 ? false : true);
}
// 检查大写字母
inline boolean isUpperCase(int c) {
    return ( isupper (c) == 0 ? false : true);
}
// 检查十六进制数
inline boolean isHexadecimalDigit(int c) {
    return ( isxdigit (c) == 0 ? false : true);
}
// 把普通字符转化为ASCII字符
inline int toAscii(int c) {
    return toascii (c);
}
// 如果使用此功能，很多人都会不满意。此函数会将重音字母转换为随机字母字符
// 如果可行，把字符转化为小写形式
inline int toLowerCase(int c) {
    return tolower (c);
}
// 如果可行，把字符转化为大写形式
inline int toUpperCase(int c) {
    return toupper (c);
}
#endif