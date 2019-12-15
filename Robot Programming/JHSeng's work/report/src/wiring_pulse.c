#include "wiring_private.h"
#include "pins_arduino.h"

// 测量引脚上脉冲的长度（以微秒为单位）； 状态为高或低，即要测量的脉冲类型。
// 可以处理2-3微秒到3分钟长度的脉冲，但是必须在脉冲开始之前至少调用几十微秒。
// 此函数在noInterrupt()上下文中使用短脉冲时效果更好
unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout) {
    // 缓存引脚的端口和位，以加快脉冲宽度测量环路并获得更高的分辨率。
    // 调用digitalRead()会产生更高的分辨率。
    uint8_t bit = digitalPinToBitMask(pin);
    uint8_t port = digitalPinToPort(pin);
    uint8_t stateMask = (state ? bit : 0);
    // 通过初始循环将超时从微秒转换为多次； 每次迭代大约需要16个时钟周期
    unsigned long maxloops = microsecondsToClockCycles(timeout) / 16;
    unsigned long width = countPulseASM(portInputRegister(port), bit, stateMask, maxloops);
    // 如果countPulseASM超时，则防止clockCyclesToMicroseconds返回虚假值
    if (width) return clockCyclesToMicroseconds(width * 16 + 16);
    else return 0;
}
// 测量引脚上脉冲的长度（以微秒为单位）； 状态为高或低，即要测量的脉冲类型。
// 可以处理2-3微秒到3分钟长度的脉冲，但是必须在脉冲开始之前至少调用几十微秒。
// 此函数依赖micros()，因此不能在noInterrupt()上下文中使用
unsigned long pulseInLong(uint8_t pin, uint8_t state, unsigned long timeout) {
    // 缓存引脚的端口和位，以加快脉冲宽度测量环路并获得更高的分辨率。
    // 调用digitalRead()会产生更高的分辨率。
    uint8_t bit = digitalPinToBitMask(pin);
    uint8_t port = digitalPinToPort(pin);
    uint8_t stateMask = (state ? bit : 0);
    unsigned long startMicros = micros();
    // 等待任何先前的脉冲结束
    while ((*portInputRegister(port) & bit) == stateMask) {
        if (micros() - startMicros > timeout) return 0;
    }
    // 等待脉冲开始
    while ((*portInputRegister(port) & bit) != stateMask) {
        if (micros() - startMicros > timeout) return 0;
    }
    unsigned long start = micros();
    // 等待脉冲停止
    while ((*portInputRegister(port) & bit) == stateMask) {
        if (micros() - startMicros > timeout) return 0;
    }
    return micros() - start;
}
