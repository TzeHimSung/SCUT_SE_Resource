/*
 Stream.cpp - adds parsing methods to Stream class
 */

#include "Arduino.h"
#include "Stream.h"

#define PARSE_TIMEOUT 1000  // 等待时间的默认值

// 带超时检测的流读入方法
int Stream::timedRead() {
    int c;
    _startMillis = millis();
    do {
        c = read();
        if (c >= 0) return c;
    } while (millis() - _startMillis < _timeout);
    return -1;     // 超时
}

// 带超时检测的流寻找方法
int Stream::timedPeek() {
    int c;
    _startMillis = millis();
    do {
        c = peek();
        if (c >= 0) return c;
    } while (millis() - _startMillis < _timeout);
    return -1;     // -1 indicates timeout
}

// 寻找流中下一个数字字符，超时则返回-1。丢弃非数字字符
int Stream::peekNextDigit(LookaheadMode lookahead, bool detectDecimal) {
    int c;
    while (1) {
        c = timedPeek();
        if ( c < 0 ||
                c == '-' ||
                (c >= '0' && c <= '9') ||
                (detectDecimal && c == '.')) return c;
        switch ( lookahead ) {
            case SKIP_NONE: return -1; // Fail code.
            case SKIP_WHITESPACE:
                switch ( c ) {
                    case ' ':
                    case '\t':
                    case '\r':
                    case '\n': break;
                    default: return -1; // Fail code.
                }
            case SKIP_ALL:
                break;
        }
        read();  // discard non-numeric
    }
}
void Stream::setTimeout(unsigned long timeout) { // 设置等待最大毫秒数
    _timeout = timeout;
}
// 找到目标字符串则返回true
bool  Stream::find(char *target) {
    return findUntil(target, strlen(target), NULL, 0);
}
// 从流中读取数据，直到找到给定长度的目标字符串
// 如果找到目标字符串，则返回true；如果超时，则返回false
bool Stream::find(char *target, size_t length) {
    return findUntil(target, length, NULL, 0);
}
// 作为查找，但如果找到终止符字符串，则搜索结束
bool Stream::findUntil(char *target, char *terminator) {
    return findUntil(target, strlen(target), terminator, strlen(terminator));
}
// 从流中读取数据，直到找到给定长度的目标字符串
// 如果找到终止符字符串，则搜索终止
// 如果找到目标字符串，则返回true；如果终止或超时，则返回false
bool Stream::findUntil(char *target, size_t targetLen, char *terminator, size_t termLen) {
    if (terminator == NULL) {
        MultiTarget t[1] = {{target, targetLen, 0}};
        return findMulti(t, 1) == 0 ? true : false;
    } else {
        MultiTarget t[2] = {{target, targetLen, 0}, {terminator, termLen, 0}};
        return findMulti(t, 2) == 0 ? true : false;
    }
}
// 从当前位置返回第一个有效的（长整数）值。前瞻确定parseInt在流中的前瞻性，可参考文件顶部的LookaheadMode枚举。
// Lookahead由第一个字符终止，该字符不是整数的有效部分。解析开始后，流中将跳过“忽略”部分。
long Stream::parseInt(LookaheadMode lookahead, char ignore) {
    bool isNegative = false;
    long value = 0;
    int c;
    c = peekNextDigit(lookahead, false);
    // 忽略非数字前导字符
    if (c < 0)
        return 0; // 超时返回0
    do {
        if (c == ignore)
            ; // 无视这一字符
        else if (c == '-')
            isNegative = true;
        else if (c >= '0' && c <= '9')       // 判断c是否为数字字符
            value = value * 10 + c - '0';
        read();  // 得到要寻找的字符
        c = timedPeek();
    } while ( (c >= '0' && c <= '9') || c == ignore );

    if (isNegative)
        value = -value;
    return value;
}

// as parseInt but returns a floating point value
float Stream::parseFloat(LookaheadMode lookahead, char ignore) {
    bool isNegative = false;
    bool isFraction = false;
    long value = 0;
    int c;
    float fraction = 1.0;
    c = peekNextDigit(lookahead, true);
    // 忽略非数字前导字符
    if (c < 0)
        return 0; // 超时返回0
    do {
        if (c == ignore)
            ; // ignore
        else if (c == '-')
            isNegative = true;
        else if (c == '.')
            isFraction = true;
        else if (c >= '0' && c <= '9')  {     // c是否为数字字符
            value = value * 10 + c - '0';
            if (isFraction)
                fraction *= 0.1;
        }
        read();  // 得到要寻找的字符
        c = timedPeek();
    } while ( (c >= '0' && c <= '9')  || (c == '.' && !isFraction) || c == ignore );
    if (isNegative)　value = -value;
    if (isFraction)　return value * fraction;
    else　return value;
}
// 从流中读取字符到缓冲区.如果已读取长度字符或超时，则终止。
// 返回放置在缓冲区中的字符数，缓冲区不是以null终止的。
size_t Stream::readBytes(char *buffer, size_t length) {
    size_t count = 0;
    while (count < length) {
        int c = timedRead();
        if (c < 0) break;
        *buffer++ = (char)c;
        count++;
    }
    return count;
}
// 作为带有终止符的readBytes。如果已读取长度字符，超时或检测到终止符，则终止。
// 返回放置在缓冲区中的字符数（0表示找不到有效数据）。
size_t Stream::readBytesUntil(char terminator, char *buffer, size_t length) {
    if (length < 1) return 0;
    size_t index = 0;
    while (index < length) {
        int c = timedRead();
        if (c < 0 || c == terminator) break;
        *buffer++ = (char)c;
        index++;
    }
    // 返回字符数量，不包含空字符
    return index;
}
// 逐字符读字符串
String Stream::readString() {
    String ret;
    int c = timedRead();
    while (c >= 0) {
        ret += (char)c;
        c = timedRead();
    }
    return ret;
}
// 一直读字符串直到遇到terminator这一字符
String Stream::readStringUntil(char terminator) {
    String ret;
    int c = timedRead();
    while (c >= 0 && c != terminator) {
        ret += (char)c;
        c = timedRead();
    }
    return ret;
}
// 寻找多个相同的字符串
int Stream::findMulti( struct Stream::MultiTarget *targets, int tCount) {
    // 任何零长度的目标字符串都会自动匹配。其余算法混乱不堪。
    for (struct MultiTarget *t = targets; t < targets + tCount; ++t) {
        if (t->len <= 0) return t - targets;
    }
    while (1) {
        int c = timedRead();
        if (c < 0) return -1;
        for (struct MultiTarget *t = targets; t < targets + tCount; ++t) {
            // 最简单的情况是，如果匹配，优先处理。
            if (c == t->str[t->index]) {
                if (++t->index == t->len)
                    return t - targets;
                else
                    continue;
            }
            // 如果不匹配，则需要往回走，看看是否可以进一步匹配。找到不匹配项时索引为0。
            if (t->index == 0) continue;
            int origIndex = t->index;
            do {
                --t->index;
                // 首先检查当前字符是否适用于新的当前索引
                if (c != t->str[t->index]) continue;
                // 如果它是唯一的字符，那么仅需检查即可
                if (t->index == 0) {
                    t->index++;
                    break;
                }
                // 否则需要检查找到的字符串的其余部分
                int diff = origIndex - t->index;
                size_t i;
                for (i = 0; i < t->index; ++i) {
                    if (t->str[i] != t->str[i + diff])
                        break;
                }
                // 如果我们成功通过上一个循环，那么我们当前索引是好的
                if (i == t->index) {
                    t->index++;
                    break;
                }
                // 否则我们尝试另一个索引
            } while (t->index);
        }
    }
    // 不可达
    return -1;
}
