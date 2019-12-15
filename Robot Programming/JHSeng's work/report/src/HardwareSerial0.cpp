#include "Arduino.h"
#include "HardwareSerial.h"
#include "HardwareSerial_private.h"

// 每个HardwareSerial都在自己的文件中定义，使用内部的任何元素时，链接器都将拉入整个文件。
// --gc-sections还会导致未使用的符号被删除，但是ISR具有“ used”属性，因此从不删除，它们也将HardwareSerial实例保留在其中。
// 将每个实例放在自己的文件中可防止链接器首先拉入任何未使用的实例。
#if defined(HAVE_HWSERIAL0)
#if defined(USART_RX_vect)
ISR(USART_RX_vect)
#elif defined(USART0_RX_vect)
ISR(USART0_RX_vect)
#elif defined(USART_RXC_vect)
ISR(USART_RXC_vect) // ATmega8
#else
#error "Don't know what the Data Received vector is called for Serial"
#endif
{
    Serial._rx_complete_irq();
}
#if defined(UART0_UDRE_vect)
ISR(UART0_UDRE_vect)
#elif defined(UART_UDRE_vect)
ISR(UART_UDRE_vect)
#elif defined(USART0_UDRE_vect)
ISR(USART0_UDRE_vect)
#elif defined(USART_UDRE_vect)
ISR(USART_UDRE_vect)
#else
#error "Don't know what the Data Register Empty vector is called for Serial"
#endif
{
    Serial._tx_udr_empty_irq();
}
#if defined(UBRRH) && defined(UBRRL)
HardwareSerial Serial(&UBRRH, &UBRRL, &UCSRA, &UCSRB, &UCSRC, &UDR);
#else
HardwareSerial Serial(&UBRR0H, &UBRR0L, &UCSR0A, &UCSR0B, &UCSR0C, &UDR0);
#endif
// 如果不使用此文件，则serialEventRun可以弱引用该函数，以防止拉入此文件。
bool Serial0_available() {
    return Serial.available();
}

#endif // HAVE_HWSERIAL0
