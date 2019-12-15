#ifndef Arduino_h
#define Arduino_h
// C标准库头文件
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
// avr-gcc附带头文件
// 这一部分的头文件可以在~/.arduino15/packages/arduino/tools/avr-gcc/7.3.0-atmel3.6.1-arduino5/avr/include/下找到
#include <avr/pgmspace.h>
#include <avr/io.h>
#include <avr/interrupt.h>
// 这一头文件定义了0~255这256个数字的二进制书写形式
#include "binary.h"
#ifdef __cplusplus
extern "C" {
#endif
void yield(void);
// 以下部分定义常用常量
// 定义高低电平
#define HIGH 0x1
#define LOW  0x0
#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2
// 定义常用变量
#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105
#define EULER 2.718281828459045235360287471352
// 定义串口和显示
#define SERIAL  0x0
#define DISPLAY 0x1
// 定义LSB优先和MSB优先
#define LSBFIRST 0
#define MSBFIRST 1
// 定义电平变化状态
#define CHANGE 1
#define FALLING 2
#define RISING 3
// 根据芯片种类进行专门定制
#if defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
#define DEFAULT 0
#define EXTERNAL 1
#define INTERNAL1V1 2
#define INTERNAL INTERNAL1V1
#elif defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
#define DEFAULT 0
#define EXTERNAL 4
#define INTERNAL1V1 8
#define INTERNAL INTERNAL1V1
#define INTERNAL2V56 9
#define INTERNAL2V56_EXTCAP 13
#else
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)
#define INTERNAL1V1 2
#define INTERNAL2V56 3
#else
#define INTERNAL 3
#endif
#define DEFAULT 1
#define EXTERNAL 0
#endif
// 覆盖旧版本不安全的abs
#ifdef abs
#undef abs
#endif
// 定义常用函数宏
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define abs(x) ((x)>0?(x):-(x))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))
// 重命名中断函数
#define interrupts() sei()
#define noInterrupts() cli()
// 定义cpu时钟宏
#define clockCyclesPerMicrosecond() ( F_CPU / 1000000L )
#define clockCyclesToMicroseconds(a) ( (a) / clockCyclesPerMicrosecond() )
#define microsecondsToClockCycles(a) ( (a) * clockCyclesPerMicrosecond() )
// 定义数字取高位低位操作
#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))
// 定义位读写操作
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))
#define bit(b) (1UL << (b))
// 补充_NOP()定义，avr-libc从1.6.2版本开始无需已补充该定义
#ifndef _NOP
#define _NOP() do { __asm__ volatile ("nop"); } while (0)
#endif
typedef unsigned int word;
typedef bool boolean;
typedef uint8_t byte;
// 声明初始化操作
void init(void);
void initVariant(void);
int atexit(void (*func)()) __attribute__((weak));
// 声明引脚操作
void pinMode(uint8_t, uint8_t);
void digitalWrite(uint8_t, uint8_t);
int digitalRead(uint8_t);
int analogRead(uint8_t);
void analogReference(uint8_t mode);
void analogWrite(uint8_t, int);
// 声明延迟和电平时间测量
unsigned long millis(void);
unsigned long micros(void);
void delay(unsigned long);
void delayMicroseconds(unsigned int us);
unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout);
unsigned long pulseInLong(uint8_t pin, uint8_t state, unsigned long timeout);
// 声明移位读入输出
void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val);
uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder);
// 声明中断服务
void attachInterrupt(uint8_t, void (*)(void), int mode);
void detachInterrupt(uint8_t);
// 声明用户自定义初始化和循环操作
void setup(void);
void loop(void);
// 获取给定虚拟引脚的硬件端口内的位位置。这一数据来源于活动板配置文件
#define analogInPinToBit(P) (P)
// 在ATmega1280上，某些端口寄存器的地址大于255, 因此我们无法将它们存储在uint8_t中
extern const uint16_t PROGMEM port_to_mode_PGM[];
extern const uint16_t PROGMEM port_to_input_PGM[];
extern const uint16_t PROGMEM port_to_output_PGM[];
extern const uint8_t PROGMEM digital_pin_to_port_PGM[];
extern const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[];
extern const uint8_t PROGMEM digital_pin_to_timer_PGM[];
// 获取给定虚拟引脚的硬件端口内的位位置。这一数据来源于活动板配置文件
// 与内联函数相比，它们作为宏的性能稍好
#define digitalPinToPort(P) ( pgm_read_byte( digital_pin_to_port_PGM + (P) ) )
#define digitalPinToBitMask(P) ( pgm_read_byte( digital_pin_to_bit_mask_PGM + (P) ) )
#define digitalPinToTimer(P) ( pgm_read_byte( digital_pin_to_timer_PGM + (P) ) )
#define analogInPinToBit(P) (P)
#define portOutputRegister(P) ( (volatile uint8_t *)( pgm_read_word( port_to_output_PGM + (P))) )
#define portInputRegister(P) ( (volatile uint8_t *)( pgm_read_word( port_to_input_PGM + (P))) )
#define portModeRegister(P) ( (volatile uint8_t *)( pgm_read_word( port_to_mode_PGM + (P))) )
// 定义非管脚、非端口、非中断状态
#define NOT_A_PIN 0
#define NOT_A_PORT 0
#define NOT_AN_INTERRUPT -1
// 定义部分常量
#ifdef ARDUINO_MAIN
#define PA 1
#define PB 2
#define PC 3
#define PD 4
#define PE 5
#define PF 6
#define PG 7
#define PH 8
#define PJ 10
#define PK 11
#define PL 12
#endif
// 定义非timer常量
#define NOT_ON_TIMER 0
#define TIMER0A 1
#define TIMER0B 2
#define TIMER1A 3
#define TIMER1B 4
#define TIMER1C 5
#define TIMER2  6
#define TIMER2A 7
#define TIMER2B 8
#define TIMER3A 9
#define TIMER3B 10
#define TIMER3C 11
#define TIMER4A 12
#define TIMER4B 13
#define TIMER4C 14
#define TIMER4D 15
#define TIMER5A 16
#define TIMER5B 17
#define TIMER5C 18

#ifdef __cplusplus
} // extern "C"到此结束
#endif

#ifdef __cplusplus
// 若运行环境为c++，还需include字符、字符串、通用串口和USB串口相关文件
#include "WCharacter.h"
#include "WString.h"
#include "HardwareSerial.h"
#include "USBAPI.h"
#if defined(HAVE_HWSERIAL0) && defined(HAVE_CDCSERIAL)
#error "Targets with both UART0 and CDC serial not supported"
#endif
// 声明word类型转换函数
uint16_t makeWord(uint16_t w);
uint16_t makeWord(byte h, byte l);
#define word(...) makeWord(__VA_ARGS__)
// 声明电平时间测量函数
unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout = 1000000L);
unsigned long pulseInLong(uint8_t pin, uint8_t state, unsigned long timeout = 1000000L);
// 声明扬声器发声和停止发声函数
void tone(uint8_t _pin, unsigned int frequency, unsigned long duration = 0);
void noTone(uint8_t _pin);
// WMath原型
long random(long);
long random(long, long);
void randomSeed(unsigned long);
long map(long, long, long, long, long);
#endif
// 此头文件定义了arduino开发板上的ATMEGA芯片引脚编号，可以在~/.arduino15/packages/arduino/hardware/avr/1.8.1/variants/standard中找到
#include "pins_arduino.h"
#endif
