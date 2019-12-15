#include "wiring_private.h"
// 下一行禁用了HardwareSerial.cpp，这使得可以支持Attiny串口和其他没有伍尔特的芯片
#if defined(HAVE_HWSERIAL0) || defined(HAVE_HWSERIAL1) || defined(HAVE_HWSERIAL2) || defined(HAVE_HWSERIAL3)
// 确保我们使用的各个位的位置都为0后缀，因此我们可以始终将UART0的值用于所有UART。
// 或者将每个UART的各种值传递给HardwareSerial构造函数也可以，但是会使得代码更长更慢。
#if !defined(TXC0)
#if defined(TXC)
// 有些芯片，例如ATmega8，没有UPE，只有PE。其他位如预期的那样命名。
#if !defined(UPE) && defined(PE)
#define UPE PE
#endif
// 在ATmega8上，没有对uart及其位进行编号，因此没有TXC0等。
#define TXC0 TXC
#define RXEN0 RXEN
#define TXEN0 TXEN
#define RXCIE0 RXCIE
#define UDRIE0 UDRIE
#define U2X0 U2X
#define UPE0 UPE
#define UDRE0 UDRE
#elif defined(TXC1)
// 有些设备有uart1但没有uart0
#define TXC0 TXC1
#define RXEN0 RXEN1
#define TXEN0 TXEN1
#define RXCIE0 RXCIE1
#define UDRIE0 UDRIE1
#define U2X0 U2X1
#define UPE0 UPE1
#define UDRE0 UDRE1
#else
#error No UART found in HardwareSerial.cpp
#endif
#endif
// 在编译时检查是否确实可以使用。其他UART为将来的硬件更改也可以使用UART0。
#if defined(TXC1) && (TXC1 != TXC0 || RXEN1 != RXEN0 || RXCIE1 != RXCIE0 || \
              UDRIE1 != UDRIE0 || U2X1 != U2X0 || UPE1 != UPE0 || \
              UDRE1 != UDRE0)
#error "Not all bit positions for UART1 are the same as for UART0"
#endif
#if defined(TXC2) && (TXC2 != TXC0 || RXEN2 != RXEN0 || RXCIE2 != RXCIE0 || \
              UDRIE2 != UDRIE0 || U2X2 != U2X0 || UPE2 != UPE0 || \
              UDRE2 != UDRE0)
#error "Not all bit positions for UART2 are the same as for UART0"
#endif
#if defined(TXC3) && (TXC3 != TXC0 || RXEN3 != RXEN0 || RXCIE3 != RXCIE0 || \
              UDRIE3 != UDRIE0 || U3X3 != U3X0 || UPE3 != UPE0 || \
              UDRE3 != UDRE0)
#error "Not all bit positions for UART3 are the same as for UART0"
#endif
// 构造函数 ////////////////////////////////////////////////////////////////
HardwareSerial::HardwareSerial(
    volatile uint8_t *ubrrh, volatile uint8_t *ubrrl,
    volatile uint8_t *ucsra, volatile uint8_t *ucsrb,
    volatile uint8_t *ucsrc, volatile uint8_t *udr) :
    _ubrrh(ubrrh), _ubrrl(ubrrl),
    _ucsra(ucsra), _ucsrb(ucsrb), _ucsrc(ucsrc),
    _udr(udr),
    _rx_buffer_head(0), _rx_buffer_tail(0),
    _tx_buffer_head(0), _tx_buffer_tail(0) {
}
// 中断处理 //////////////////////////////////////////////////////////////
void HardwareSerial::_rx_complete_irq(void) {
    if (bit_is_clear(*_ucsra, UPE0)) {
        // 如果有空间，则读取字节并将其存储在缓冲区中。没有奇偶校验错误
        unsigned char c = *_udr;
        rx_buffer_index_t i = (unsigned int)(_rx_buffer_head + 1) % SERIAL_RX_BUFFER_SIZE;
        // 如果我们应该将收到的字符存储到该位置。紧接在尾部之前（表示头部将前进到尾部当前位置），缓冲区会溢出。因此不会写字符或抬头。
        if (i != _rx_buffer_tail) {
            _rx_buffer[_rx_buffer_head] = c;
            _rx_buffer_head = i;
        }
    } else {
        // 奇偶校验错误，读取字节但将其丢弃
        *_udr;
    };
}
#endif // whole file
