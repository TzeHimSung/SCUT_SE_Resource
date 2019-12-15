#ifndef Stream_h
#define Stream_h
#include <inttypes.h>
#include "Print.h"
// 这里的枚举为parseInt(), parseFloat()提供了选项。可以使用这里的规则直到找到第一个有效字符为止。
enum LookaheadMode {
    SKIP_ALL,       // 忽略所有无效字符
    SKIP_NONE,      // 不跳过任何内容，且不会触碰流，除非第一个等待的字符有效
    SKIP_WHITESPACE // 仅跳过制表符、空格、换行符和回车
};
#define NO_IGNORE_CHAR  '\x01' // 在有效的ASCII数字字段中找不到字符
class Stream : public Print {
protected:
    unsigned long _timeout;      // 中止计时读取之前等待下一个字符的毫秒数
    unsigned long _startMillis;  // 用于超时测量
    int timedRead();    // 读取流超时
    int timedPeek();    // 等待流超时
    int peekNextDigit(LookaheadMode lookahead, bool detectDecimal); // 返回流中的下一个数字，如果超时则返回-1
public:
    virtual int available() = 0;
    virtual int read() = 0;
    virtual int peek() = 0;
    Stream() {
        _timeout = 1000;
    }
// 解析流的方法
    void setTimeout(unsigned long timeout);  // 设置等待流数据的最大毫秒数，默认为1秒
    unsigned long getTimeout(void) {
        return _timeout;
    }
    bool find(char *target);   // 从流中读取数据，直到找到目标字符串
    bool find(uint8_t *target) {
        return find ((char *)target);
    }
    // 找到目标字符串则返回true，反之为false
    bool find(char *target, size_t length);   // 从流中读取数据，直到找到给定长度的目标字符串
    bool find(uint8_t *target, size_t length) {
        return find ((char *)target, length);
    }
    // 如果找到目标字符串，则返回true；如果超时，则返回false
    bool find(char target) {
        return find (&target, 1);
    }
    bool findUntil(char *target, char *terminator);   // 如果找到终止符，则搜索结束
    bool findUntil(uint8_t *target, char *terminator) {
        return findUntil((char *)target, terminator);
    }
    bool findUntil(char *target, size_t targetLen, char *terminate, size_t termLen);   // 如果找到终止字符串，搜索将结束
    bool findUntil(uint8_t *target, size_t targetLen, char *terminate, size_t termLen) {
        return findUntil((char *)target, targetLen, terminate, termLen);
    }
    long parseInt(LookaheadMode lookahead = SKIP_ALL, char ignore = NO_IGNORE_CHAR);
    // 从当前位置返回第一个有效的long int值，前瞻确定parseInt在流中的前瞻性，可以参阅LookaheadMode枚举。
    // Lookahead由第一个字符终止，该字符不是整数的有效部分。解析开始后，流中将跳过“忽略”部分。

    float parseFloat(LookaheadMode lookahead = SKIP_ALL, char ignore = NO_IGNORE_CHAR);
    // float版本的parseInt

    size_t readBytes( char *buffer, size_t length); // 从流中读取字符到缓冲区
    size_t readBytes( uint8_t *buffer, size_t length) {
        return readBytes((char *)buffer, length);
    }
    //如果已读取长度字符或超时，则终止。返回放置在缓冲区中的字符数。

    size_t readBytesUntil( char terminator, char *buffer, size_t length); // 作为带有终止符的读取字符
    size_t readBytesUntil( char terminator, uint8_t *buffer, size_t length) {
        return readBytesUntil(terminator, (char *)buffer, length);
    }
    // 如果已读取长度字符，超时或检测到终止符，则终止。返回放置在缓冲区中的字符数。

    // Arduino字符串函数在这里添加
    String readString();
    String readStringUntil(char terminator);

protected:
    long parseInt(char ignore) {
        return parseInt(SKIP_ALL, ignore);
    }
    float parseFloat(char ignore) {
        return parseFloat(SKIP_ALL, ignore);
    }
    // 这些重载是为了与派生的任何类兼容，以自定义忽略字符流式传输并使用parseFloat / Int。
    // 保持公共API很简单，这些重载仍受保护。

    struct MultiTarget {
        const char *str;  // 模式串
        size_t len;       // 模式串长度
        size_t index;     // 索引
    };

    // 这可以让您搜索任意数量的字符串。返回最先找到的目标的索引；如果发生超时，则返回-1。
    int findMulti(struct MultiTarget *targets, int tCount);
};

#undef NO_IGNORE_CHAR
#endif
