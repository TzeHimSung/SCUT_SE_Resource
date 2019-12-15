#define ARDUINO_MAIN
#include "wiring_private.h"
#include "pins_arduino.h"

void pinMode(uint8_t pin, uint8_t mode) {
    uint8_t bit = digitalPinToBitMask(pin);
    uint8_t port = digitalPinToPort(pin);
    volatile uint8_t *reg, *out;
    if (port == NOT_A_PIN) return;
    reg = portModeRegister(port);
    out = portOutputRegister(port);
    if (mode == INPUT) {
        uint8_t oldSREG = SREG;
        cli();
        *reg &= ~bit;
        *out &= ~bit;
        SREG = oldSREG;
    } else if (mode == INPUT_PULLUP) {
        uint8_t oldSREG = SREG;
        cli();
        *reg &= ~bit;
        *out |= bit;
        SREG = oldSREG;
    } else {
        uint8_t oldSREG = SREG;
        cli();
        *reg |= bit;
        SREG = oldSREG;
    }
}
// 强制此内联使调用者不必将自己的内容推入堆栈。 这是一个很好的性能胜利，每个用户只比调用多占用1个字节。 （在168上将占用更多字节。）
// 但是不应该将其移入pinMode吗？ 似乎很傻地检查和执行每个数字读或写。
// Mark Sproul：
// -删除了内联。 在atmega1280上节省170字节
// -更改为switch语句； 增加了32个字节，但更易于阅读和维护。
// -添加了更多的#ifdefs，现在可以为atmega645编译
static void turnOffPWM(uint8_t timer) {
    switch (timer) {
#if defined(TCCR1A) && defined(COM1A1)
        case TIMER1A:   cbi(TCCR1A, COM1A1);    break;
#endif
#if defined(TCCR1A) && defined(COM1B1)
        case TIMER1B:   cbi(TCCR1A, COM1B1);    break;
#endif
#if defined(TCCR1A) && defined(COM1C1)
        case TIMER1C:   cbi(TCCR1A, COM1C1);    break;
#endif

#if defined(TCCR2) && defined(COM21)
        case  TIMER2:   cbi(TCCR2, COM21);      break;
#endif

#if defined(TCCR0A) && defined(COM0A1)
        case  TIMER0A:  cbi(TCCR0A, COM0A1);    break;
#endif

#if defined(TCCR0A) && defined(COM0B1)
        case  TIMER0B:  cbi(TCCR0A, COM0B1);    break;
#endif
#if defined(TCCR2A) && defined(COM2A1)
        case  TIMER2A:  cbi(TCCR2A, COM2A1);    break;
#endif
#if defined(TCCR2A) && defined(COM2B1)
        case  TIMER2B:  cbi(TCCR2A, COM2B1);    break;
#endif

#if defined(TCCR3A) && defined(COM3A1)
        case  TIMER3A:  cbi(TCCR3A, COM3A1);    break;
#endif
#if defined(TCCR3A) && defined(COM3B1)
        case  TIMER3B:  cbi(TCCR3A, COM3B1);    break;
#endif
#if defined(TCCR3A) && defined(COM3C1)
        case  TIMER3C:  cbi(TCCR3A, COM3C1);    break;
#endif

#if defined(TCCR4A) && defined(COM4A1)
        case  TIMER4A:  cbi(TCCR4A, COM4A1);    break;
#endif
#if defined(TCCR4A) && defined(COM4B1)
        case  TIMER4B:  cbi(TCCR4A, COM4B1);    break;
#endif
#if defined(TCCR4A) && defined(COM4C1)
        case  TIMER4C:  cbi(TCCR4A, COM4C1);    break;
#endif
#if defined(TCCR4C) && defined(COM4D1)
        case TIMER4D:   cbi(TCCR4C, COM4D1);    break;
#endif

#if defined(TCCR5A)
        case  TIMER5A:  cbi(TCCR5A, COM5A1);    break;
        case  TIMER5B:  cbi(TCCR5A, COM5B1);    break;
        case  TIMER5C:  cbi(TCCR5A, COM5C1);    break;
#endif
    }
}
void digitalWrite(uint8_t pin, uint8_t val) {
    uint8_t timer = digitalPinToTimer(pin);
    uint8_t bit = digitalPinToBitMask(pin);
    uint8_t port = digitalPinToPort(pin);
    volatile uint8_t *out;
    if (port == NOT_A_PIN) return;
    // 如果该引脚支持PWM输出，则需要在进行数字写入之前将其关闭。
    if (timer != NOT_ON_TIMER) turnOffPWM(timer);
    out = portOutputRegister(port);
    uint8_t oldSREG = SREG;
    cli();
    if (val == LOW) *out &= ~bit;
    else *out |= bit;
    SREG = oldSREG;
}

int digitalRead(uint8_t pin) {
    uint8_t timer = digitalPinToTimer(pin);
    uint8_t bit = digitalPinToBitMask(pin);
    uint8_t port = digitalPinToPort(pin);
    if (port == NOT_A_PIN) return LOW;
    // 如果该引脚支持PWM输出，则需要在获得数字读数之前将其关闭。
    if (timer != NOT_ON_TIMER) turnOffPWM(timer);
    if (*portInputRegister(port) & bit) return HIGH;
    return LOW;
}
