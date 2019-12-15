#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <util/atomic.h>
#include "Arduino.h"
#include "HardwareSerial.h"
#include "HardwareSerial_private.h"
// 下一行将禁用整个HardwareSerial.cpp，
// 这样就可以在没有uart的情况下支持Attiny系列和任何其他芯片
#if defined(HAVE_HWSERIAL0) || defined(HAVE_HWSERIAL1) || defined(HAVE_HWSERIAL2) || defined(HAVE_HWSERIAL3)
// SerialEvent函数比较弱，因此当用户未定义它们时，链接器仅将其地址设置为0（在下面进行检查）。
// Serialx_available只是Serialx.available（）的包装，但我们可以较弱地引用它，因此我们不会将其全部HardwareSerial实例化（如果用户也未引用它）。
#if defined(HAVE_HWSERIAL0)
void serialEvent() __attribute__((weak));
bool Serial0_available() __attribute__((weak));
#endif
#if defined(HAVE_HWSERIAL1)
void serialEvent1() __attribute__((weak));
bool Serial1_available() __attribute__((weak));
#endif
#if defined(HAVE_HWSERIAL2)
void serialEvent2() __attribute__((weak));
bool Serial2_available() __attribute__((weak));
#endif
#if defined(HAVE_HWSERIAL3)
void serialEvent3() __attribute__((weak));
bool Serial3_available() __attribute__((weak));
#endif
void serialEventRun(void) {
#if defined(HAVE_HWSERIAL0)
    if (Serial0_available && serialEvent && Serial0_available()) serialEvent();
#endif
#if defined(HAVE_HWSERIAL1)
    if (Serial1_available && serialEvent1 && Serial1_available()) serialEvent1();
#endif
#if defined(HAVE_HWSERIAL2)
    if (Serial2_available && serialEvent2 && Serial2_available()) serialEvent2();
#endif
#if defined(HAVE_HWSERIAL3)
    if (Serial3_available && serialEvent3 && Serial3_available()) serialEvent3();
#endif
}
// 大型TX缓冲区需要时，使用宏可保护关键部分
#if (SERIAL_TX_BUFFER_SIZE>256)
#define TX_BUFFER_ATOMIC ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
#else
#define TX_BUFFER_ATOMIC
#endif
// 实际中断处理程序 //////////////////////////////////////////////////////////////
void HardwareSerial::_tx_udr_empty_irq(void) {
    // 如果启用了中断，则输出中必须有更多数据缓冲以发送下一个字节
    unsigned char c = _tx_buffer[_tx_buffer_tail];
    _tx_buffer_tail = (_tx_buffer_tail + 1) % SERIAL_TX_BUFFER_SIZE;
    *_udr = c;
    // 清除TXC位“可以通过在其位写入1来清除location”。这样可以确保flush()直到字节恢复后才会返回。
    // 实际上是写的。其他的r/w位被保留，并且作为零写入其余部分。
#ifdef MPCM0
    *_ucsra = ((*_ucsra) & ((1 << U2X0) | (1 << MPCM0))) | (1 << TXC0);
#else
    *_ucsra = ((*_ucsra) & ((1 << U2X0) | (1 << TXC0)));
#endif
    if (_tx_buffer_head == _tx_buffer_tail) {
        // 缓冲区空，所以停止中断
        cbi(*_ucsrb, UDRIE0);
    }
}
// public方法 //////////////////////////////////////////////////////////////
void HardwareSerial::begin(unsigned long baud, byte config) {
    // 使用u2x模式
    uint16_t baud_setting = (F_CPU / 4 / baud - 1) / 2;
    *_ucsra = 1 << U2X0;
    // 57600的硬编码异常，用于与引导加载程序兼容。这随Duemilanove和以前的主板以及固件一起提供给在Uno和Mega 2560的8U2。
    // 此外，baud_setting无法大于4095，如果波特率太高，请切换回非u2x模式。
    if (((F_CPU == 16000000UL) && (baud == 57600)) || (baud_setting > 4095)) {
        *_ucsra = 0;
        baud_setting = (F_CPU / 8 / baud - 1) / 2;
    }
    // 分配baud_setting，又称ubrr（USART波特率寄存器）
    *_ubrrh = baud_setting >> 8;
    *_ubrrl = baud_setting;
    _written = false;
    // 设置数据位，奇偶校验和停止位
#if defined(__AVR_ATmega8__)
    config |= 0x80; // 选择UCSRC寄存器（与UBRRH共享）
#endif
    *_ucsrc = config;
    sbi(*_ucsrb, RXEN0);
    sbi(*_ucsrb, TXEN0);
    sbi(*_ucsrb, RXCIE0);
    cbi(*_ucsrb, UDRIE0);
}
void HardwareSerial::end() {
    // 等待传出数据的传输
    flush();
    cbi(*_ucsrb, RXEN0);
    cbi(*_ucsrb, TXEN0);
    cbi(*_ucsrb, RXCIE0);
    cbi(*_ucsrb, UDRIE0);
    // 清除接收到的数据
    _rx_buffer_head = _rx_buffer_tail;
}
// 检测串口是否可用
int HardwareSerial::available(void) {
    return ((unsigned int)(SERIAL_RX_BUFFER_SIZE + _rx_buffer_head - _rx_buffer_tail)) % SERIAL_RX_BUFFER_SIZE;
}
int HardwareSerial::peek(void) {
    if (_rx_buffer_head == _rx_buffer_tail) {
        return -1;
    } else {
        return _rx_buffer[_rx_buffer_tail];
    }
}
// 读数据流
int HardwareSerial::read(void) {
    // 如果头部不在尾巴前面，就没有任何字符
    if (_rx_buffer_head == _rx_buffer_tail) {
        return -1;
    } else {
        unsigned char c = _rx_buffer[_rx_buffer_tail];
        _rx_buffer_tail = (rx_buffer_index_t)(_rx_buffer_tail + 1) % SERIAL_RX_BUFFER_SIZE;
        return c;
    }
}
// 检查是否可写
int HardwareSerial::availableForWrite(void) {
    tx_buffer_index_t head;
    tx_buffer_index_t tail;
    TX_BUFFER_ATOMIC {
        head = _tx_buffer_head;
        tail = _tx_buffer_tail;
    }
    if (head >= tail) return SERIAL_TX_BUFFER_SIZE - 1 - head + tail;
    return tail - head - 1;
}
// 清除缓冲区
void HardwareSerial::flush() {
    // 如果我们从未写过字节，则无需刷新。 这个特别情况需要使用大小写，因为无法强制（转换完成）使得TXC在初始化期间为1。
    if (!_written)
        return;
    while (bit_is_set(*_ucsrb, UDRIE0) || bit_is_clear(*_ucsra, TXC0)) {
        if (bit_is_clear(SREG, SREG_I) && bit_is_set(*_ucsrb, UDRIE0))
            // 中断被全局禁止，但空DR中断应被允许，所以设置空DR flag给当前死锁
            if (bit_is_set(*_ucsra, UDRE0)) _tx_udr_empty_irq();
    }
    // 如果到达这里，则不再排队(禁用DRIE)，硬件完成传输（已设置TXC）。
}
// 串口写
size_t HardwareSerial::write(uint8_t c) {
    _written = true;
    // 如果缓冲区和数据寄存器为空，则只需写入字节到数据寄存器并完成。
    // 此快捷方式有助于显着提高高效率的数据速率（>500kbit/s）比特率，其中中断开销会变慢。
    if (_tx_buffer_head == _tx_buffer_tail && bit_is_set(*_ucsra, UDRE0)) {
        // 如果在写入UDR和前一个字节之前清除TXC。在写入UDR之前完成，TXC将被置位，但一个字节仍在传输，导致flush（）返回得太早。因此，必须先进行UDR编写。
        // 必须以原子方式完成编写UDR和清除TC的操作，否则中断可能会延迟TXC清零，因此将字节写入UDR。在清除TXC之前被发送（设置TXC）。 然后TXC将
        // 当没有字节时清除，导致flush（）挂起。
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            *_udr = c;
#ifdef MPCM0
            *_ucsra = ((*_ucsra) & ((1 << U2X0) | (1 << MPCM0))) | (1 << TXC0);
#else
            *_ucsra = ((*_ucsra) & ((1 << U2X0) | (1 << TXC0)));
#endif
        }
        return 1;
    }
    tx_buffer_index_t i = (_tx_buffer_head + 1) % SERIAL_TX_BUFFER_SIZE;
    // 如果输出缓冲区已满，则除了等待中断处理程序将其清空
    while (i == _tx_buffer_tail) {
        if (bit_is_clear(SREG, SREG_I)) {
            // 中断被禁用，因此我们必须轮询数据自己注册空标志。 如果已设置，则假装发生中断并调用处理程序以释放我们的空间。
            if (bit_is_set(*_ucsra, UDRE0))
                _tx_udr_empty_irq();
        } else {
            // 中断处理程序将为我们释放空间
        }
    }
    _tx_buffer[_tx_buffer_head] = c;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        _tx_buffer_head = i;
        sbi(*_ucsrb, UDRIE0);
    }
    return 1;
}
#endif
