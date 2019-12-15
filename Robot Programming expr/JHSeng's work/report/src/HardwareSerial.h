/*
  HardwareSerial.h - Hardware serial library for Wiring
*/

#ifndef HardwareSerial_h
#define HardwareSerial_h
#include <inttypes.h>
#include "Stream.h"
// 定义用于缓冲传入串行数据的常量和变量，使用环形缓冲区，其中head是位置的索引，向其写入下一个传入字符，并且尾部为的索引从中读取的位置。
// 建议缓冲区的大小为二的幂次以优化环形缓冲区的所有模运算
// 警告：当缓冲区大小增加到>256时，缓冲区索引变量会自动增加大小，但多余的部分所需的原子防护未实现。通常来说可以正常工作，但偶尔会有串行行为异常。
#if ((RAMEND - RAMSTART) < 1023)
#define SERIAL_TX_BUFFER_SIZE 16
#else
#define SERIAL_TX_BUFFER_SIZE 64
#endif
#endif
#if !defined(SERIAL_RX_BUFFER_SIZE)
#if ((RAMEND - RAMSTART) < 1023)
#define SERIAL_RX_BUFFER_SIZE 16
#else
#define SERIAL_RX_BUFFER_SIZE 64
#endif
#endif
#if (SERIAL_TX_BUFFER_SIZE>256)
typedef uint16_t tx_buffer_index_t;
#else
typedef uint8_t tx_buffer_index_t;
#endif
#if  (SERIAL_RX_BUFFER_SIZE>256)
typedef uint16_t rx_buffer_index_t;
#else
typedef uint8_t rx_buffer_index_t;
#endif
// 为Serial.begin(baud，config)定义配置
#define SERIAL_5N1 0x00
#define SERIAL_6N1 0x02
#define SERIAL_7N1 0x04
#define SERIAL_8N1 0x06
#define SERIAL_5N2 0x08
#define SERIAL_6N2 0x0A
#define SERIAL_7N2 0x0C
#define SERIAL_8N2 0x0E
#define SERIAL_5E1 0x20
#define SERIAL_6E1 0x22
#define SERIAL_7E1 0x24
#define SERIAL_8E1 0x26
#define SERIAL_5E2 0x28
#define SERIAL_6E2 0x2A
#define SERIAL_7E2 0x2C
#define SERIAL_8E2 0x2E
#define SERIAL_5O1 0x30
#define SERIAL_6O1 0x32
#define SERIAL_7O1 0x34
#define SERIAL_8O1 0x36
#define SERIAL_5O2 0x38
#define SERIAL_6O2 0x3A
#define SERIAL_7O2 0x3C
#define SERIAL_8O2 0x3E

class HardwareSerial : public Stream {
protected:
    volatile uint8_t *const _ubrrh;
    volatile uint8_t *const _ubrrl;
    volatile uint8_t *const _ucsra;
    volatile uint8_t *const _ucsrb;
    volatile uint8_t *const _ucsrc;
    volatile uint8_t *const _udr;
    // 从begin()开始是否将任何字节写入UART
    bool _written;
    volatile rx_buffer_index_t _rx_buffer_head;
    volatile rx_buffer_index_t _rx_buffer_tail;
    volatile tx_buffer_index_t _tx_buffer_head;
    volatile tx_buffer_index_t _tx_buffer_tail;
    // 不能将任何成员放在这些缓冲区之后，因为只有第一个可使用ldd快速访问此结构的32个字节指令
    unsigned char _rx_buffer[SERIAL_RX_BUFFER_SIZE];
    unsigned char _tx_buffer[SERIAL_TX_BUFFER_SIZE];
public:
    inline HardwareSerial(
        volatile uint8_t *ubrrh, volatile uint8_t *ubrrl,
        volatile uint8_t *ucsra, volatile uint8_t *ucsrb,
        volatile uint8_t *ucsrc, volatile uint8_t *udr);
    void begin(unsigned long baud) {
        begin(baud, SERIAL_8N1);
    }
    void begin(unsigned long, uint8_t);
    void end();
    virtual int available(void);
    virtual int peek(void);
    virtual int read(void);
    virtual int availableForWrite(void);
    virtual void flush(void);
    virtual size_t write(uint8_t);
    inline size_t write(unsigned long n) {
        return write((uint8_t)n);
    }
    inline size_t write(long n) {
        return write((uint8_t)n);
    }
    inline size_t write(unsigned int n) {
        return write((uint8_t)n);
    }
    inline size_t write(int n) {
        return write((uint8_t)n);
    }
    using Print::write; // 从Print中获取write(str)和write(buf，size)
    operator bool() {
        return true;
    }
    // 中断处理程序，这不应在外部调用
    inline void _rx_complete_irq(void);
    void _tx_udr_empty_irq(void);
};

#if defined(UBRRH) || defined(UBRR0H)
extern HardwareSerial Serial;
#define HAVE_HWSERIAL0
#endif
#if defined(UBRR1H)
extern HardwareSerial Serial1;
#define HAVE_HWSERIAL1
#endif
#if defined(UBRR2H)
extern HardwareSerial Serial2;
#define HAVE_HWSERIAL2
#endif
#if defined(UBRR3H)
extern HardwareSerial Serial3;
#define HAVE_HWSERIAL3
#endif
extern void serialEventRun(void) __attribute__((weak));
#endif
