#ifndef String_class_h
#define String_class_h
#ifdef __cplusplus
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <avr/pgmspace.h>
// 使用此类编译程序时，以下gcc参数将大大提高性能和内存（RAM）效率，通常几乎没有代码大小的增加
// -felide-constructors
// -std = c++0x
class __FlashStringHelper;
#define F(string_literal) (reinterpret_cast<const __FlashStringHelper *>(PSTR(string_literal)))
// 用于保存串联结果的继承类，这些结果对象被假定为可被后续串联写入。
class StringSumHelper;
// String类
class String {
    // 使用函数指针使得无效字符串存在，且脱离bool的复杂度
    typedef void (String::*StringIfHelperType)() const;
    void StringIfHelper() const {}
public:
    // 如果初始值为null或无效，或者内存分配失败，该字符串将被标记为无效
    String(const char *cstr = "");
    String(const String &str);
    String(const __FlashStringHelper *str);
#if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__)
    String(String &&rval);
    String(StringSumHelper &&rval);
#endif
    explicit String(char c);
    explicit String(unsigned char, unsigned char base = 10);
    explicit String(int, unsigned char base = 10);
    explicit String(unsigned int, unsigned char base = 10);
    explicit String(long, unsigned char base = 10);
    explicit String(unsigned long, unsigned char base = 10);
    explicit String(float, unsigned char decimalPlaces = 2);
    explicit String(double, unsigned char decimalPlaces = 2);
    ~String(void);
    // 内存管理。如果成功则返回true，失败返回false。成功的情况下reverse(0)将验证无效的字符串
    unsigned char reserve(unsigned int size);
    inline unsigned int length(void) const {
        return len;
    }
    // 创建分配值的副本。如果值为null则无效，否则如果内存分配失败，则字符串也标记为无效
    String &operator = (const String &rhs);
    String &operator = (const char *cstr);
    String &operator = (const __FlashStringHelper *str);
#if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__)
    String &operator = (String &&rval);
    String &operator = (StringSumHelper &&rval);
#endif
    //字符串拼接(带有内置类型)。成功返回true，失败返回false（在这种情况下，字符串保持不变）。如果参数为null或无效，则拼接被认为是不成功的。
    unsigned char concat(const String &str);
    unsigned char concat(const char *cstr);
    unsigned char concat(char c);
    unsigned char concat(unsigned char c);
    unsigned char concat(int num);
    unsigned char concat(unsigned int num);
    unsigned char concat(long num);
    unsigned char concat(unsigned long num);
    unsigned char concat(float num);
    unsigned char concat(double num);
    unsigned char concat(const __FlashStringHelper *str);
    //如果没有足够的存储空间供连接的值使用，则字符串将保持不变，但这不会以任何方式发出信号
    String &operator += (const String &rhs) {
        concat(rhs);
        return (*this);
    }
    String &operator += (const char *cstr) {
        concat(cstr);
        return (*this);
    }
    String &operator += (char c) {
        concat(c);
        return (*this);
    }
    String &operator += (unsigned char num) {
        concat(num);
        return (*this);
    }
    String &operator += (int num) {
        concat(num);
        return (*this);
    }
    String &operator += (unsigned int num) {
        concat(num);
        return (*this);
    }
    String &operator += (long num) {
        concat(num);
        return (*this);
    }
    String &operator += (unsigned long num) {
        concat(num);
        return (*this);
    }
    String &operator += (float num) {
        concat(num);
        return (*this);
    }
    String &operator += (double num) {
        concat(num);
        return (*this);
    }
    String &operator += (const __FlashStringHelper *str) {
        concat(str);
        return (*this);
    }
    friend StringSumHelper &operator + (const StringSumHelper &lhs, const String &rhs);
    friend StringSumHelper &operator + (const StringSumHelper &lhs, const char *cstr);
    friend StringSumHelper &operator + (const StringSumHelper &lhs, char c);
    friend StringSumHelper &operator + (const StringSumHelper &lhs, unsigned char num);
    friend StringSumHelper &operator + (const StringSumHelper &lhs, int num);
    friend StringSumHelper &operator + (const StringSumHelper &lhs, unsigned int num);
    friend StringSumHelper &operator + (const StringSumHelper &lhs, long num);
    friend StringSumHelper &operator + (const StringSumHelper &lhs, unsigned long num);
    friend StringSumHelper &operator + (const StringSumHelper &lhs, float num);
    friend StringSumHelper &operator + (const StringSumHelper &lhs, double num);
    friend StringSumHelper &operator + (const StringSumHelper &lhs, const __FlashStringHelper *rhs);
    // 字符串比较，只适用于(W)Strings和strings
    operator StringIfHelperType() const {
        return buffer ? &String::StringIfHelper : 0;
    }
    int compareTo(const String &s) const;
    unsigned char equals(const String &s) const;
    unsigned char equals(const char *cstr) const;
    unsigned char operator == (const String &rhs) const {
        return equals(rhs);
    }
    unsigned char operator == (const char *cstr) const {
        return equals(cstr);
    }
    unsigned char operator != (const String &rhs) const {
        return !equals(rhs);
    }
    unsigned char operator != (const char *cstr) const {
        return !equals(cstr);
    }
    unsigned char operator <  (const String &rhs) const;
    unsigned char operator >  (const String &rhs) const;
    unsigned char operator <= (const String &rhs) const;
    unsigned char operator >= (const String &rhs) const;
    unsigned char equalsIgnoreCase(const String &s) const;
    unsigned char startsWith( const String &prefix) const;
    unsigned char startsWith(const String &prefix, unsigned int offset) const;
    unsigned char endsWith(const String &suffix) const;
    // 字符串内容访问
    char charAt(unsigned int index) const;
    void setCharAt(unsigned int index, char c);
    char operator [] (unsigned int index) const;
    char &operator [] (unsigned int index);
    void getBytes(unsigned char *buf, unsigned int bufsize, unsigned int index = 0) const;
    void toCharArray(char *buf, unsigned int bufsize, unsigned int index = 0) const {
        getBytes((unsigned char *)buf, bufsize, index);
    }
    const char *c_str() const {
        return buffer;
    }
    char *begin() {
        return buffer;
    }
    char *end() {
        return buffer + length();
    }
    const char *begin() const {
        return c_str();
    }
    const char *end() const {
        return c_str() + length();
    }
    // 搜索
    int indexOf( char ch ) const;
    int indexOf( char ch, unsigned int fromIndex ) const;
    int indexOf( const String &str ) const;
    int indexOf( const String &str, unsigned int fromIndex ) const;
    int lastIndexOf( char ch ) const;
    int lastIndexOf( char ch, unsigned int fromIndex ) const;
    int lastIndexOf( const String &str ) const;
    int lastIndexOf( const String &str, unsigned int fromIndex ) const;
    String substring( unsigned int beginIndex ) const {
        return substring(beginIndex, len);
    };
    String substring( unsigned int beginIndex, unsigned int endIndex ) const;
    // 字符串修改
    void replace(char find, char replace);
    void replace(const String &find, const String &replace);
    void remove(unsigned int index);
    void remove(unsigned int index, unsigned int count);
    void toLowerCase(void);
    void toUpperCase(void);
    void trim(void);
    // 解析与转换
    long toInt(void) const;
    float toFloat(void) const;
    double toDouble(void) const;
protected:
    char *buffer;           // 字符数组
    unsigned int capacity;  // 数组长度
    unsigned int len;       // 字符串长度
protected:
    void init(void);
    void invalidate(void);
    unsigned char changeBuffer(unsigned int maxStrLen);
    unsigned char concat(const char *cstr, unsigned int length);
    // 字符串复制
    String &copy(const char *cstr, unsigned int length);
    String &copy(const __FlashStringHelper *pstr, unsigned int length);
#if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__)
    void move(String &rhs);
#endif
};

class StringSumHelper : public String {
public:
    StringSumHelper(const String &s) : String(s) {}
    StringSumHelper(const char *p) : String(p) {}
    StringSumHelper(char c) : String(c) {}
    StringSumHelper(unsigned char num) : String(num) {}
    StringSumHelper(int num) : String(num) {}
    StringSumHelper(unsigned int num) : String(num) {}
    StringSumHelper(long num) : String(num) {}
    StringSumHelper(unsigned long num) : String(num) {}
    StringSumHelper(float num) : String(num) {}
    StringSumHelper(double num) : String(num) {}
};

#endif  // __cplusplus
#endif  // String_class_h
